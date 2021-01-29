/*---------------------------------------------------------------------------*
  Project:  NitroSDK - WBT - include
  File:     context.h

  Copyright 2006,2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

 *---------------------------------------------------------------------------*/
#ifndef	NITRO_WBT_CONTEXT_H_
#define	NITRO_WBT_CONTEXT_H_


/*===========================================================================*/

#include <nitro/platform.h>
#include <nitro/misc.h>
#include <nitro/math/math.h>
#include <nitro/wbt.h>


#ifdef	__cplusplus
extern "C" {
#endif


/*---------------------------------------------------------------------------*/
/* declarations */


/*****************************************************************************
 * �d�g�ɏ��ʐM�t�H�[�}�b�g.
 *****************************************************************************/

/* REQ_SYNC �R�}���h�����t�H�[�}�b�g�\���� */
typedef struct WBTPacketRequestSyncFormat
{
    PLATFORM_LE16 peer_packet;
    PLATFORM_LE16 own_packet;
}
PLATFORM_STRUCT_PADDING_FOOTER WBTPacketRequestSyncFormat;

/* RES_SYNC �R�}���h�����t�H�[�}�b�g�\���� */
typedef struct WBTPacketResponseSyncFormat
{
    PLATFORM_LE16 block_total;
    PLATFORM_LE16 peer_packet;
    PLATFORM_LE16 own_packet;
}
PLATFORM_STRUCT_PADDING_FOOTER WBTPacketResponseSyncFormat;

/* REQ_USERDATA �R�}���h�����t�H�[�}�b�g�\���� */
typedef struct WBTPacketRequestUserDataFormat
{
    PLATFORM_LE8 length;
    PLATFORM_LE8 buffer[WBT_SIZE_USER_DATA];
}
WBTPacketRequestUserDataFormat;

/* REQ_GETBLOCK_DONE �R�}���h�����t�H�[�}�b�g�\���� */
typedef struct WBTPacketRequestGetBlockDoneFormat
{
    PLATFORM_LE32 id;
}
WBTPacketRequestGetBlockDoneFormat;

/* RES_GETBLOCK_DONE �R�}���h�����t�H�[�}�b�g�\���� */
typedef struct WBTPacketResponseGetBlockDoneFormat
{
    PLATFORM_LE32 id;
}
WBTPacketResponseGetBlockDoneFormat;

/* REQ_GETBLOCK �R�}���h�����t�H�[�}�b�g�\���� */
typedef struct WBTPacketRequestGetBlockFormat
{
    PLATFORM_LE32 id;
    PLATFORM_LE32 index;
}
WBTPacketRequestGetBlockFormat;

/* RES_GETBLOCK �R�}���h�����t�H�[�}�b�g�\���� */
typedef struct WBTPacketResponseGetBlockFormat
{
    PLATFORM_LE32 id;
    PLATFORM_LE32 index;
}
WBTPacketResponseGetBlockFormat;

/* �p�P�b�g�w�b�_�t�H�[�}�b�g�\���� */
typedef struct WBTPacketHeaderFormat
{
    PLATFORM_LE8 command;
    PLATFORM_LE16 bitmap;
    PLATFORM_LE8 counter;
}
PLATFORM_STRUCT_PADDING_FOOTER WBTPacketHeaderFormat;

/* �p�P�b�g�t�H�[�}�b�g�\���� */
typedef struct WBTPacketFormat
{
    /* �p�P�b�g�w�b�_ */
    WBTPacketHeaderFormat header;
    /* �R�}���h�ɂ���Ă͌㑱�̈��������݂��� */
    union
    {
        u8      argument[10];
        WBTPacketRequestSyncFormat req_sync;
        WBTPacketResponseSyncFormat res_sync;
        WBTPacketRequestUserDataFormat req_userdata;
        WBTPacketRequestGetBlockDoneFormat req_getblock_done;
        WBTPacketResponseGetBlockDoneFormat res_getblock_done;
        WBTPacketRequestGetBlockFormat req_getblock;
        WBTPacketResponseGetBlockFormat res_getblock;
        u8      for_compiler[10];
    } PLATFORM_STRUCT_PADDING_FOOTER /* unnamed */ ;
    /* �R�}���h�ɂ���Ă͂���ɉϒ��̌㑱���������݂��� */
}
PLATFORM_STRUCT_PADDING_FOOTER WBTPacketFormat;

PLATFORM_COMPILER_ASSERT(sizeof(WBTPacketHeaderFormat) == 4);
PLATFORM_COMPILER_ASSERT(sizeof(WBTPacketRequestSyncFormat) == 4);
PLATFORM_COMPILER_ASSERT(sizeof(WBTPacketResponseSyncFormat) == 6);
PLATFORM_COMPILER_ASSERT(sizeof(WBTPacketRequestUserDataFormat) == 10);
PLATFORM_COMPILER_ASSERT(sizeof(WBTPacketRequestGetBlockDoneFormat) == 4);
PLATFORM_COMPILER_ASSERT(sizeof(WBTPacketResponseGetBlockDoneFormat) == 4);
PLATFORM_COMPILER_ASSERT(sizeof(WBTPacketRequestGetBlockFormat) == 8);
PLATFORM_COMPILER_ASSERT(sizeof(WBTPacketResponseGetBlockFormat) == 8);
PLATFORM_COMPILER_ASSERT(sizeof(WBTPacketFormat) == 14);


/*****************************************************************************
 * ���[�J���\����.
 *****************************************************************************/

struct WBTContext;
struct WBTCommandList;

/* �R�}���h�R�[���o�b�N�v���g�^�C�v */
typedef void (*WBTEventCallback)(void*, WBTCommand*);

/* �R�}���h���X�g�\���� */
typedef struct WBTCommandList
{
    struct WBTCommandList  *next;
    WBTCommand              command;
    WBTEventCallback        callback;
}
WBTCommandList;


/* �e�@���eAID���Ƃɕێ����Ă����ŐV�̎�M�� */
typedef struct WBTRecvToken
{
    u8      token_command;
    u8      token_peer_cmd_counter;
    u8      last_peer_cmd_counter;

    u8      dummy[1];

    /*
     * ������ WBT_CMD_REQ_GET_BLOCK* �n�ł����������Ă��Ȃ�.
     * �������ɎQ�Ƃ���.
     */
    u32     token_block_id;
    s32     token_block_seq_no;
}
WBTRecvToken;

/* �e�@���eAID���Ƃɕێ����Ă����e�탏�[�N�ϐ� */
typedef struct WBTPacketBitmap
{
    s32     length;
    void   *buffer;
    s32     count;
    s32     total;
    u32    *bitmap;
    s32     current;
}
WBTPacketBitmap;

/* wbt_data.c �Ŏg�p����R�}���h�Ǘ��\���� */
typedef struct WBTContext
{
    /* �������̃R�}���h���X�g�Ƌ󂫂̃R�}���h�v�[�� */
    WBTCommandList *command;
    WBTCommandList *command_pool;

    /* �C�ӂ̃��[�U��`�l */
    void               *userdata;
    WBTEventCallback    callback;

    /*
     * �V�X�e���R�[���o�b�N�p�o�b�t�@.
     * �ꎞ�I�ɂ����g�p���Ȃ���, 156 BYTE ������̂ŃR���e�L�X�g�ɕێ�.
     */
    WBTCommand system_cmd;

    /* �e AID �̏�� */
    struct
    {
        WBTRecvToken recv_token;
        WBTPacketBitmap pkt_bmp;
    }
    peer_param[16];

    /* ���g�̒ʐM��� */
    int     my_aid;                    /* ���ǂ�AID */
    s16     peer_data_packet_size;     /* ����ǂ̃p�P�b�g�f�[�^���̃T�C�Y */
    s16     my_data_packet_size;       /* ���ǂ̃p�P�b�g�f�[�^���̃T�C�Y */
    WBTBlockInfoList *list;            /* �o�^�ς݃f�[�^�u���b�N���X�g */
    u8      my_command_counter;        /* ���[�U�[�R�}���h���s�J�E���^ */
    u8      padding[3];
    int     last_target_aid;

    /* �u���b�N�ԍ��̑��M���� */
    u32     last_block_id;
    s32     last_seq_no_1;
    s32     last_seq_no_2;

    /* ���ݗv������M���Ă���r�b�g�}�b�v */
    int     req_bitmap;

    /* GetBlockInfo �p�̃r�b�g�}�b�v */
    u32     binfo_bitmap[16][MATH_ROUNDUP(sizeof(WBTBlockInfo), sizeof(u32)) / sizeof(u32)];
}
WBTContext;


/*---------------------------------------------------------------------------*/
/* functions */

/*---------------------------------------------------------------------------*
  Name:         WBT_InitContext

  Description:  WBT�\���̂�������.

  Arguments:    work              WBTContext�\����.
                userdata          �C�ӂ̃��[�U��`�l.
                callback          �V�X�e���R�[���o�b�N.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void    WBT_InitContext(WBTContext *work, void *userdata, WBTEventCallback callback);

/*---------------------------------------------------------------------------*
  Name:         WBT_StartParent

  Description:  WBT��e�@�Ƃ��ĊJ�n.

  Arguments:    work              WBTContext�\����.
                own               ���g��MP���M�p�P�b�g�T�C�Y.
                peer              �����MP���M�p�P�b�g�T�C�Y.

  Returns:      None.
 *---------------------------------------------------------------------------*/
PLATFORM_ATTRIBUTE_INLINE
void    WBT_StartParent(WBTContext *work, int own, int peer)
{
    SDK_MIN_ASSERT(own, WBT_PACKET_SIZE_MIN);
    SDK_MIN_ASSERT(peer, WBT_PACKET_SIZE_MIN);
    work->my_aid = 0;
    work->my_data_packet_size = (s16)(own - WBT_PACKET_SIZE_MIN);
    work->peer_data_packet_size = (s16)(peer - WBT_PACKET_SIZE_MIN);
}

/*---------------------------------------------------------------------------*
  Name:         WBT_StartChild

  Description:  WBT���q�@�Ƃ��ĊJ�n.

  Arguments:    work              WBTContext�\����.
                aid               ���g��AID.

  Returns:      None.
 *---------------------------------------------------------------------------*/
PLATFORM_ATTRIBUTE_INLINE
void    WBT_StartChild(WBTContext *work, int aid)
{
    work->my_data_packet_size = 0;
    work->peer_data_packet_size = 0;
    work->my_aid = aid;
}

/*---------------------------------------------------------------------------*
  Name:         WBT_ResetContext

  Description:  WBT���ď�����.

  Arguments:    work              WBTContext�\����.
                callback          �V�X�e���R�[���o�b�N.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void    WBT_ResetContext(WBTContext *work, WBTEventCallback callback);

/*---------------------------------------------------------------------------*
  Name:         WBT_CallPacketSendHook

  Description:  ���M�p�P�b�g�f�[�^�𐶐����邽�߂̃t�b�N�֐�.

  Arguments:    work              WBTContext�\����.
                buffer            �f�[�^�i�[�o�b�t�@.
                length            �o�b�t�@�T�C�Y.

  Returns:      �������ꂽ�p�P�b�g�T�C�Y.
 *---------------------------------------------------------------------------*/
int     WBT_CallPacketSendHook(WBTContext *work, void *buffer, int length, BOOL is_parent);

/*---------------------------------------------------------------------------*
  Name:         WBT_CallPacketRecvHook

  Description:  ��M�p�P�b�g�f�[�^�����.

  Arguments:    work              WBTContext�\����.
                aid               �f�[�^���M����AID
                buffer            ��M�f�[�^�o�b�t�@.
                length            ��M�f�[�^��.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void    WBT_CallPacketRecvHook(WBTContext *work, int aid, const void *buffer, int length);

/*---------------------------------------------------------------------------*
  Name:         WBT_GetUserData

  Description:  �R���e�L�X�g�Ɋ֘A�t����ꂽ���[�U��`�l���擾.

  Arguments:    work              WBTContext�\����.

  Returns:      ���ݏ������̃R�}���h���܂��� NULL.
 *---------------------------------------------------------------------------*/
PLATFORM_ATTRIBUTE_INLINE
void   *WBT_GetUserData(const WBTContext *work)
{
    return work->userdata;
}

/*---------------------------------------------------------------------------*
  Name:         WBT_GetAid

  Description:  �ݒ肳�ꂽAID�l���擾.

  Arguments:    work              WBTContext�\����.

  Returns:      �ݒ肳�ꂽAID�l.
 *---------------------------------------------------------------------------*/
PLATFORM_ATTRIBUTE_INLINE
int WBT_GetAid(const WBTContext *work)
{
    return work->my_aid;
}

/*---------------------------------------------------------------------------*
  Name:         WBT_GetOwnPacketLength

  Description:  ���݂̎��g�̑��M�p�P�b�g�T�C�Y���擾.

  Arguments:    work              WBTContext�\����.

  Returns:      ���݂̎��g�̑��M�p�P�b�g�T�C�Y.
 *---------------------------------------------------------------------------*/
PLATFORM_ATTRIBUTE_INLINE
int WBT_GetOwnPacketLength(const WBTContext *work)
{
    return work->my_data_packet_size;
}

/*---------------------------------------------------------------------------*
  Name:         WBT_GetPeerPacketLength

  Description:  ���݂̑���̑��M�p�P�b�g�T�C�Y���擾.

  Arguments:    work              WBTContext�\����.

  Returns:      ���݂̑���̑��M�p�P�b�g�T�C�Y.
 *---------------------------------------------------------------------------*/
PLATFORM_ATTRIBUTE_INLINE
int WBT_GetPeerPacketLength(const WBTContext *work)
{
    return work->peer_data_packet_size;
}

/*---------------------------------------------------------------------------*
  Name:         WBT_GetParentPacketLength

  Description:  ���݂̐e�@���M�p�P�b�g�T�C�Y���擾.

  Arguments:    work              WBTContext�\����.

  Returns:      ���݂̐e�@���M�p�P�b�g�T�C�Y.
 *---------------------------------------------------------------------------*/
PLATFORM_ATTRIBUTE_INLINE
int WBT_GetParentPacketLength(const WBTContext *work)
{
    return (work->my_aid == 0) ? work->my_data_packet_size : work->peer_data_packet_size;
}

/*---------------------------------------------------------------------------*
  Name:         WBT_GetRegisteredCount

  Description:  �o�^�ς݂̃f�[�^�u���b�N�������擾.

  Arguments:    work              WBTContext�\����.

  Returns:      �o�^�ς݂̃f�[�^�u���b�N����.
 *---------------------------------------------------------------------------*/
int     WBT_GetRegisteredCount(const WBTContext * work);

/*---------------------------------------------------------------------------*
  Name:         WBT_GetCurrentCommandList

  Description:  ���ݏ������̃R�}���h���擾.

  Arguments:    work              WBTContext�\����.

  Returns:      ���ݏ������̃R�}���h���܂��� NULL.
 *---------------------------------------------------------------------------*/
PLATFORM_ATTRIBUTE_INLINE
WBTCommandList *WBT_GetCurrentCommandList(const WBTContext *work)
{
    return work->command;
}

/*---------------------------------------------------------------------------*
  Name:         WBT_GetCurrentCommand

  Description:  ���ݏ������̃R�}���h���擾.

  Arguments:    work              WBTContext�\����.

  Returns:      ���ݏ������̃R�}���h���܂��� NULL.
 *---------------------------------------------------------------------------*/
PLATFORM_ATTRIBUTE_INLINE
WBTCommand *WBT_GetCurrentCommand(const WBTContext *work)
{
    WBTCommandList *list = work->command;
    return list ? &list->command : NULL;
}

/*---------------------------------------------------------------------------*
  Name:         WBT_GetBitmapLength

  Description:  �u���b�N�]���̐���ɕK�v�ȃr�b�g�}�b�v�o�b�t�@�T�C�Y���擾.

  Arguments:    work              WBTContext�\����.
                length            �]������u���b�N�̍ő�T�C�Y.

  Returns:      �K�v�ȃr�b�g�}�b�v�o�b�t�@�̃T�C�Y.
 *---------------------------------------------------------------------------*/
int WBT_GetBitmapLength(const WBTContext *work, int length);

/*---------------------------------------------------------------------------*
  Name:         WBT_AddCommandPool

  Description:  �R�}���h�v�[���ɐV�K�̃��X�g��ǉ�.

  Arguments:    work              WBTContext�\����.
                list              �R�}���h���X�g�\���̂̔z��.
                count             �z��̗v�f��.

  Returns:      None.
 *---------------------------------------------------------------------------*/
PLATFORM_ATTRIBUTE_INLINE
void    WBT_AddCommandPool(WBTContext *work, WBTCommandList *list, int count)
{
    while (--count >= 0)
    {
        list->next = work->command_pool;
        work->command_pool = list++;
    }
}

/*---------------------------------------------------------------------------*
  Name:         WBT_AllocCommandList

  Description:  �R�}���h�v�[������1�̃��X�g��V�K�m��.

  Arguments:    work              WBTContext�\����.

  Returns:      �m�ۂ��ꂽ�V�K�̃R�}���h���X�g���܂��� NULL.
 *---------------------------------------------------------------------------*/
PLATFORM_ATTRIBUTE_INLINE
WBTCommandList *WBT_AllocCommandList(WBTContext *work)
{
    WBTCommandList *list = work->command_pool;
    if (list)
    {
        work->command_pool = list->next;
        list->next = NULL;
    }
    return list;
}

/*---------------------------------------------------------------------------*
  Name:         WBT_SetPacketLength

  Description:  �p�P�b�g�T�C�Y��ύX.
                �e�@�̂ݎg�p�\.

  Arguments:    work              WBTContext�\����.
                own               ���g��MP���M�p�P�b�g�T�C�Y.
                peer              �����MP���M�p�P�b�g�T�C�Y.

  Returns:      �ݒ�ɐ�������� TRUE.
 *---------------------------------------------------------------------------*/
BOOL    WBT_SetPacketLength(WBTContext *work, int own, int peer);

/*---------------------------------------------------------------------------*
  Name:         WBT_CreateCommandSYNC

  Description:  "SYNC" �R�}���h���𐶐�����.

  Arguments:    work              WBTContext�\����.
                list              �R�}���h�����i�[���邽�߂ɗp�ӂ��ꂽ���X�g.

  Returns:      None.
 *---------------------------------------------------------------------------*/
PLATFORM_ATTRIBUTE_INLINE
void WBT_CreateCommandSYNC(WBTContext *work, WBTCommandList *list)
{
    (void)work;
    list->command.command = WBT_CMD_REQ_SYNC;
}

/*---------------------------------------------------------------------------*
  Name:         WBT_CreateCommandINFO

  Description:  "INFO" �R�}���h�������X�g�ɐݒ肷��.

  Arguments:    work              WBTContext�\����.
                list              �R�}���h�����i�[���邽�߂ɗp�ӂ��ꂽ���X�g.
                index             �擾����u���b�N��������
                                  �o�^���X�g�擪����̃C���f�b�N�X.
                buffer_table      �擾�����u���b�N�����i�[����
                                  WBTBlockInfoTable �|�C���^�̃e�[�u��.

  Returns:      None.
 *---------------------------------------------------------------------------*/
PLATFORM_ATTRIBUTE_INLINE
void WBT_CreateCommandINFO(WBTContext *work, WBTCommandList *list,
                           int index, const WBTBlockInfoTable *buffer_table)
{
    WBTGetBlockCallback *arg = &list->command.get;
    arg->block_id = (u32)index;
    arg->recv_data_size = sizeof(WBTBlockInfo);
    {
        int     i;
        for (i = 0; i < 16; ++i)
        {
            arg->pkt_bmp_table.packet_bitmap[i] = work->binfo_bitmap[i];
            arg->recv_buf_table.recv_buf[i] = (u8 *)buffer_table->block_info[i];
        }
    }
    list->command.command = WBT_CMD_REQ_GET_BLOCKINFO;
}

/*---------------------------------------------------------------------------*
  Name:         WBT_CreateCommandGET

  Description:  "GET" �R�}���h�������X�g�ɐݒ肷��.

  Arguments:    work              WBTContext�\����.
                list              �R�}���h�����i�[���邽�߂ɗp�ӂ��ꂽ���X�g.
                id                �擾����u���b�N��ID.
                length            �擾����u���b�N�f�[�^��.
                buffer_table      �擾�����u���b�N�f�[�^���i�[����
                                  WBTRecvBufTable �|�C���^�̃e�[�u��.
                bitmap_table      �����ł̃u���b�N�]������̂��߂ɕK�v��
                                  ��M�󋵊Ǘ��o�b�t�@�̃e�[�u��.

  Returns:      None.
 *---------------------------------------------------------------------------*/
PLATFORM_ATTRIBUTE_INLINE
void WBT_CreateCommandGET(WBTContext *work, WBTCommandList * list,
                          u32 id, u32 length,
                          const WBTRecvBufTable *buffer_table,
                          WBTPacketBitmapTable *bitmap_table)
{
    WBTGetBlockCallback *arg = &list->command.get;
    (void)work;
    arg->block_id = id;
    arg->recv_data_size = length;
    arg->recv_buf_table = *buffer_table;
    arg->pkt_bmp_table = *bitmap_table;
    list->command.command = WBT_CMD_REQ_GET_BLOCK;
}

/*---------------------------------------------------------------------------*
  Name:         WBT_CreateCommandMSG

  Description:  "MSG" �R�}���h�������X�g�ɐݒ肷��.

  Arguments:    work              WBTContext�\����.
                list              �R�}���h�����i�[���邽�߂ɗp�ӂ��ꂽ���X�g.
                buffer            ���M�f�[�^���i�[���ꂽ�o�b�t�@.
                                  �o�b�t�@�̓��e�͂��̊֐����ł̂ݎQ�Ƃ����.
                length            ���M�f�[�^��.
                                  WBT_SIZE_USER_DATA �ȉ��ł���K�v������.

  Returns:      None.
 *---------------------------------------------------------------------------*/
PLATFORM_ATTRIBUTE_INLINE
void    WBT_CreateCommandMSG(WBTContext *work, WBTCommandList *list,
                             const void *buffer, u32 length)
{
    WBTRecvUserDataCallback *arg = &list->command.user_data;
    (void)work;
    SDK_MINMAX_ASSERT(length, 0, WBT_SIZE_USER_DATA);
    arg->size = (u8)length;
    MI_CpuCopy8(buffer, arg->data, length);
    list->command.command = WBT_CMD_REQ_USER_DATA;
}

/*---------------------------------------------------------------------------*
  Name:         WBT_PostCommand

  Description:  �R�}���h�𔭍s���ăR�}���h�L���[�ɒǉ�.

  Arguments:    work              WBTContext�\����.
                list              �R�}���h��񂪊i�[���ꂽ�\����.
                                  �R�}���h�����܂Ń��C�u���������ŊǗ������.
                bitmap            �R�}���h���s�Ώۂ�AID�r�b�g�}�b�v.
                callback          �R�}���h�����R�[���o�b�N. �s�v�Ȃ� NULL.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void    WBT_PostCommand(WBTContext *work, WBTCommandList *list, u16 bitmap,
                        WBTEventCallback callback);

/*---------------------------------------------------------------------------*
  Name:         WBT_PostCommandSYNC

  Description:  "SYNC" �R�}���h���𔭍s����.

  Arguments:    context           WBTContext�\����.
                bitmap            �R�}���h���s�Ώۂ�AID�r�b�g�}�b�v.
                callback          �R�}���h�����R�[���o�b�N. �s�v�Ȃ� NULL.

  Returns:      �󂫂̃R�}���h���X�g�����݂��ăR�}���h���s�ɐ���������TRUE.
 *---------------------------------------------------------------------------*/
PLATFORM_ATTRIBUTE_INLINE
BOOL    WBT_PostCommandSYNC(WBTContext *context, int bitmap, WBTEventCallback callback)
{
    WBTCommandList *list = WBT_AllocCommandList(context);
    if (list)
    {
        WBT_CreateCommandSYNC(context, list);
        WBT_PostCommand(context, list, (u16)bitmap, callback);
    }
    return (list != NULL);
}

/*---------------------------------------------------------------------------*
  Name:         WBT_PostCommandINFO

  Description:  "INFO" �R�}���h���𔭍s����.

  Arguments:    context           WBTContext�\����.
                bitmap            �R�}���h���s�Ώۂ�AID�r�b�g�}�b�v.
                callback          �R�}���h�����R�[���o�b�N. �s�v�Ȃ� NULL.
                index             �擾����u���b�N��������
                                  �o�^���X�g�擪����̃C���f�b�N�X.
                buffer_table      �擾�����u���b�N�����i�[����
                                  WBTBlockInfoTable �|�C���^�̃e�[�u��.

  Returns:      �󂫂̃R�}���h���X�g�����݂��ăR�}���h���s�ɐ���������TRUE.
 *---------------------------------------------------------------------------*/
PLATFORM_ATTRIBUTE_INLINE
BOOL    WBT_PostCommandINFO(WBTContext *context, int bitmap, WBTEventCallback callback,
                            int index, const WBTBlockInfoTable *buffer_table)
{
    WBTCommandList *list = WBT_AllocCommandList(context);
    if (list)
    {
        WBT_CreateCommandINFO(context, list, index, buffer_table);
        WBT_PostCommand(context, list, (u16)bitmap, callback);
    }
    return (list != NULL);
}

/*---------------------------------------------------------------------------*
  Name:         WBT_PostCommandGET

  Description:  "GET" �R�}���h���𔭍s����.

  Arguments:    context           WBTContext�\����.
                bitmap            �R�}���h���s�Ώۂ�AID�r�b�g�}�b�v.
                callback          �R�}���h�����R�[���o�b�N. �s�v�Ȃ� NULL.
                id                �擾����u���b�N��ID.
                length            �擾����u���b�N�f�[�^��.
                buffer_table      �擾�����u���b�N�f�[�^���i�[����
                                  WBTRecvBufTable �|�C���^�̃e�[�u��.
                bitmap_table      �����ł̃u���b�N�]������̂��߂ɕK�v��
                                  ��M�󋵊Ǘ��o�b�t�@�̃e�[�u��.

  Returns:      �󂫂̃R�}���h���X�g�����݂��ăR�}���h���s�ɐ���������TRUE.
 *---------------------------------------------------------------------------*/
PLATFORM_ATTRIBUTE_INLINE
BOOL    WBT_PostCommandGET(WBTContext *context, int bitmap, WBTEventCallback callback,
                           u32 id, u32 length, const WBTRecvBufTable *buffer_table,
                           WBTPacketBitmapTable *bitmap_table)
{
    WBTCommandList *list = WBT_AllocCommandList(context);
    if (list)
    {
        WBT_CreateCommandGET(context, list, id, length, buffer_table, bitmap_table);
        WBT_PostCommand(context, list, (u16)bitmap, callback);
    }
    return (list != NULL);
}

/*---------------------------------------------------------------------------*
  Name:         WBT_PostCommandMSG

  Description:  "MSG" �R�}���h���𔭍s����.

  Arguments:    context           WBTContext�\����.
                bitmap            �R�}���h���s�Ώۂ�AID�r�b�g�}�b�v.
                callback          �R�}���h�����R�[���o�b�N. �s�v�Ȃ� NULL.
                buffer            ���M�f�[�^���i�[���ꂽ�o�b�t�@.
                                  �o�b�t�@�̓��e�͂��̊֐����ł̂ݎQ�Ƃ����.
                length            ���M�f�[�^��.
                                  WBT_SIZE_USER_DATA �ȉ��ł���K�v������.

  Returns:      �󂫂̃R�}���h���X�g�����݂��ăR�}���h���s�ɐ���������TRUE.
 *---------------------------------------------------------------------------*/
PLATFORM_ATTRIBUTE_INLINE
BOOL    WBT_PostCommandMSG(WBTContext *context, int bitmap, WBTEventCallback callback,
                           const void *buffer, u32 length)
{
    WBTCommandList *list = WBT_AllocCommandList(context);
    if (list)
    {
        WBT_CreateCommandMSG(context, list, buffer, length);
        WBT_PostCommand(context, list, (u16)bitmap, callback);
    }
    return (list != NULL);
}

/*---------------------------------------------------------------------------*
  Name:         WBT_CancelCommand

  Description:  ���ݏ������̃R�}���h�𒆎~.

  Arguments:    work              WBTContext�\����.
                bitmap            �R�}���h�𒆎~���鑊��.

  Returns:      ���ۂɒ��~���ꂽ����������r�b�g�}�b�v.
 *---------------------------------------------------------------------------*/
int     WBT_CancelCommand(WBTContext * work, int bitmap);

/*---------------------------------------------------------------------------*
  Name:         WBT_GetDownloadProgress

  Description:  �u���b�N�]���̐i���󋵂��擾.

  Arguments:    work              WBTContext�\����.
                id                ��M�u���b�NID.
                aid               ��M��AID.
                current           ��M�ς݃p�P�b�g���̊i�[��.
                total             �p�P�b�g�����̊i�[��.

  Returns:      None.
                �u���b�N�]����ԂɂȂ���� current, total �Ƃ� 0 ��Ԃ�.
 *---------------------------------------------------------------------------*/
void    WBT_GetDownloadProgress(const WBTContext * work, u32 id, int aid, int *current, int *total);

/*---------------------------------------------------------------------------*
  Name:         WBT_RegisterBlockInfo

  Description:  �V�K�Ƀf�[�^�u���b�N��o�^����.

  Arguments:    work              WBTContext�\����.
                list              �o�^�Ɏg�p���郊�X�g�\����.
                                  Unregister �ŉ������܂Ń��C�u�������g�p����.
                id                �f�[�^�u���b�N�Ɋ֘A�t�����ӂ�ID.
                userinfo          �f�[�^�u���b�N�Ɋ֘A�t���郆�[�U��`���.
                                  ���̃|�C���^���w����͂��̊֐����ł̂ݎQ�Ƃ���.
                                  �s�v�ȏꍇ�� NULL ���w�肷�邱�Ƃ��ł���.
                buffer            �u���b�N�f�[�^���i�[�����o�b�t�@.
                                  NULL ���w�肷���, �K�v�ɉ����ă��C�u��������
                                  WBT_CMD_PREPARE_SEND_DATA �R�[���o�b�N���ʒm�����.
                length            �u���b�N�f�[�^�̃T�C�Y.
                                  buffer �� NULL ���w�肷��ꍇ�ł����Ă�
                                  ���̒l�͐������w�肵�Ă����K�v������.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void    WBT_RegisterBlockInfo(WBTContext * work, WBTBlockInfoList *list, u32 id,
                              const void *userinfo, const void *buffer, int length);

/*---------------------------------------------------------------------------*
  Name:         WBT_UnregisterBlockInfo

  Description:  �o�^�ς݂̃f�[�^�u���b�N���������.

  Arguments:    work              WBTContext�\����.
                id                �������f�[�^�u���b�N�Ɋ֘A�t����ꂽ��ӂ�ID.

  Returns:      ����������X�g�\���̂܂��� NULL.
 *---------------------------------------------------------------------------*/
WBTBlockInfoList *WBT_UnregisterBlockInfo(WBTContext * work, u32 id);


/*===========================================================================*/

#ifdef	__cplusplus
}          /* extern "C" */
#endif

#endif /* NITRO_WBT_CONTEXT_H_ */

/*---------------------------------------------------------------------------*

  $Log: context.h,v $
  Revision 1.3  2007/10/04 03:52:18  yosizaki
  fix about extern "C"

  Revision 1.2  2007/08/22 05:22:32  yosizaki
  fix dependency.

  Revision 1.1  2007/04/10 08:20:14  yosizaki
  initial upload.

  $NoKeywords: $
 *---------------------------------------------------------------------------*/
