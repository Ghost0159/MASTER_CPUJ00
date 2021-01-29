/*---------------------------------------------------------------------------*
  Project:  NitroSDK - WBT - demos - wbt-1
  File:     bt.c

  Copyright 2003-2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: bt.c,v $
  Revision 1.19  2006/01/18 02:11:19  kitase_hirotake
  do-indent

  Revision 1.18  2005/12/15 01:00:39  yosizaki
  fix about NOT_USE_ALLOC.

  Revision 1.17  2005/02/28 05:26:01  yosizaki
  do-indent.

  Revision 1.16  2005/02/18 09:26:13  yosizaki
  fix around hidden warnings.

  Revision 1.15  2004/10/28 11:06:19  yosizaki
  delete waste 'extern'.

  Revision 1.14  2004/10/25 11:14:39  yasu
  Support SDK_NO_MESSAGE

  Revision 1.13  2004/10/19 10:27:02  miya
  Port��M�R�[���o�b�N�̎g�p�ƃv�����g����NITRO��ʏ�ɕ\������悤�ɕύX

  Revision 1.12  2004/10/04 08:40:35  miya
  add WBT_CancelCurrentCommand

  Revision 1.11  2004/10/04 05:49:53  miya
  change transfer size according to the programing guideline

  Revision 1.10  2004/09/13 09:12:22  miya
  OS_TPrintf���g�p����悤�ɕύX

  Revision 1.9  2004/09/13 06:39:43  miya
  �}�N���ǉ�

  Revision 1.8  2004/09/01 04:00:15  miya
  PutBlock,RequestRegisterBlock,ResponseRegisterBlock,Done�R�}���h�폜

  Revision 1.7  2004/09/01 00:16:56  miya
  ���[�U�[�f�[�^�̃T�C�Y�ύX

  Revision 1.6  2004/08/28 11:26:56  miya
  WBT_PutUserData�֐��ǉ�

  Revision 1.5  2004/08/27 07:08:27  miya
  ���O�Ȃǂ̕ύX

  Revision 1.4  2004/08/26 23:54:28  miya
  ���O�ύX�Ȃ�

  Revision 1.3  2004/08/24 02:41:57  miya
  �R�����g�ǉ�

  Revision 1.2  2004/08/23 12:39:40  miya
  UI�I�ɖ��ɂȂ肻���ȂƂ���̕ύX

  Revision 1.1  2004/08/23 09:47:33  yosizaki
  (none)

  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#include <nitro.h>
#include <nitro/wbt.h>
#include "wc.h"
#include "bt.h"
#include "text.h"

#define NOT_USE_ALLOC

#ifndef	SDK_NO_MESSAGE
#define	__MY_LINE__		__LINE__
#else
#define	__MY_LINE__		0
#endif

/* �q�@�p��M�u���b�N����f�[�^ */
static int request_block_num;
static int block_info_num = 0;
static OSTick start_tick;
static int bt_loop_count = 0;
static int bt_running = 0;

/* �q�@�p��M�p�o�b�t�@���X */
static WBTBlockInfoTable block_info_table;
static WBTRecvBufTable recv_buf_table;
static WBTPacketBitmapTable recv_buf_packet_bmp_table;
static WBTAidBitmap tbmp = 1;          /* ���N�G�X�g�v������i�e�j */

/* �q�@�p���[�U�[�f�[�^���M�o�b�t�@ */
static u8 user_data[WBT_SIZE_USER_DATA];

#ifdef NOT_USE_ALLOC
WBTBlockInfo block_info[WBT_NUM_OF_AID][NUM_OF_BT_LIST];
u8      recv_buf[WBT_NUM_OF_AID][BT_DATA_SIZE];
u32    
    recv_pkt_bmp_buf[WBT_NUM_OF_AID][WBT_PACKET_BITMAP_SIZE(BT_DATA_SIZE, BT_PARENT_PACKET_SIZE)];
#endif

/* �e�@�p�u���b�N�f�[�^ */
static WBTBlockInfoList bt_list[NUM_OF_BT_LIST];
static u8 bt_data[NUM_OF_BT_LIST][BT_DATA_SIZE];

static u8 user_char_id[NUM_OF_BT_LIST][WBT_USER_ID_LEN] = {
    "BT text information area 0",
    "BT text information area 1",
    "BT text information area 2",
#if 0
    "BT text information area 3",
    "BT text information area 4"
#endif
};


const char *command_str[] = {
    "REQ_NONE",
    "REQ_WAIT",
    "REQ_SYNC",
    "RES_SYNC",
    "REQ_GET_BLOCK",
    "RES_GET_BLOCK",
    "REQ_GET_BLOCKINFO",
    "RES_GET_BLOCKINFO",
    "REQ_GET_BLOCK_DONE",
    "RES_GET_BLOCK_DONE",
    "REQ_USER_DATA",
    "RES_USER_DATA",
    "SYSTEM_CALLBACK",
    "PREPARE_SEND_DATA",
    "REQ_ERROR",
    "RES_ERROR",
    "CANCEL"
};




static int strlen(char *str)
{
    int     i = 0;
    while (1)
    {
        if (*str != '\0')
        {
            str++;
            i++;
        }
        else
        {
            break;
        }
    }
    return i;
}


/* �e�@�p�u���b�N�o�^�֐� */
void bt_register_blocks(void)
{
    int     i;
    char   *end_string = "This is BlockTransfer test data contents end\n";
    char   *ptr;
    int     offset;

    for (i = 0; i < NUM_OF_BT_LIST; i++)
    {
        offset = BT_DATA_SIZE - (strlen(end_string) + 1);
        *(s32 *)(&(bt_data[i][0])) = offset;
        (void)OS_SPrintf((char *)&(bt_data[i][4]),
                         "This is BlockTransfer test data contents start %d\n", i);

        ptr = (char *)(&(bt_data[i][0]) + offset);
        (void)OS_SPrintf((char *)ptr, "%s", end_string);

        (void)WBT_RegisterBlock(&(bt_list[i]), (u32)(10000 + i) /* id 1000�ȉ��͕s�� */ ,
                                user_char_id[i], &(bt_data[i][0]), BT_DATA_SIZE, 0);
    }
}


/* �q�@�p�u���b�N�]���I���֐� */
void bt_stop(void)
{
    bt_running = 0;
    (void)WBT_CancelCurrentCommand(0xffff);
}


/* �q�@�p�u���b�N�]���J�n�֐� */
void bt_start(void)
{
    OSIntrMode enabled;
    static int init_flag = FALSE;
    int     i;

    enabled = OS_DisableInterrupts();

    if (bt_running)
    {
        (void)OS_RestoreInterrupts(enabled);
        return;
    }
    bt_running = 1;

#ifdef NOT_USE_ALLOC

    if (init_flag == FALSE)
    {
        init_flag = TRUE;
        for (i = 0; i < WBT_NUM_OF_AID; i++)
        {
            block_info_table.block_info[i] = &(block_info[i][0]);
            recv_buf_table.recv_buf[i] = &(recv_buf[i][0]);
            recv_buf_packet_bmp_table.packet_bitmap[i] = &(recv_pkt_bmp_buf[i][0]);
        }
    }

#else

    mfprintf(tc[2], "child bt start\n");

    if (init_flag == FALSE)

    {
        init_flag = TRUE;
        /* �q�@�p��M�p�o�b�t�@���X�̏����� */
        for (i = 0; i < WBT_NUM_OF_AID; i++)
        {
            block_info_table.block_info[i] = NULL;
            recv_buf_table.recv_buf[i] = NULL;
            recv_buf_packet_bmp_table.packet_bitmap[i] = NULL;
        }
    }

    for (i = 0; i < WBT_NUM_OF_AID; i++)
    {
        if (block_info_table.block_info[i] != NULL)
        {
            OS_Free(block_info_table.block_info[i]);
            block_info_table.block_info[i] = NULL;
        }
        if (recv_buf_table.recv_buf[i] != NULL)
        {
            OS_Free(recv_buf_table.recv_buf[i]);
            recv_buf_table.recv_buf[i] = NULL;
        }
        if (recv_buf_packet_bmp_table.packet_bitmap[i] != NULL)
        {
            OS_Free(recv_buf_packet_bmp_table.packet_bitmap[i]);
            recv_buf_packet_bmp_table.packet_bitmap[i] = NULL;
        }
    }
#endif

    (void)OS_RestoreInterrupts(enabled);

    (void)WBT_RequestSync(tbmp,        /* Sync�v�������鑊��i�e�@�̂ݕ����j */
                          bt_callback  /* �I�����R�[���o�b�N */
        );
}


/* �u���b�N�]���ėp�R�[���o�b�N�֐� */
void bt_callback(void *arg)
{

    WBTCommand *uc = (WBTCommand *)arg;
    int     peer_aid = WBT_AidbitmapToAid(uc->peer_bmp);        /* �ʐM����ǂ�AID */
    // mfprintf(tc[2],"aid = %d\n", peer_aid);

    switch (uc->event)
    {
    case WBT_CMD_RES_SYNC:
        /* WBT_RequestSync�I�� */
        request_block_num = 0;
        block_info_num = uc->sync.num_of_list;  /* ����̎����Ă���u���b�N�� */
        mfprintf(tc[2], "blockinfo num = %d my_packet_size = %d peer_packet_size = %d\n", block_info_num, uc->sync.my_packet_size,      /* ����̑��M�f�[�^�T�C�Y */
                 uc->sync.peer_packet_size      /* ������̑��M�f�[�^�T�C�Y */
            );
#ifndef NOT_USE_ALLOC
        mfprintf(tc[2], "info buf alloc %d\n", peer_aid);
        /* �u���b�N���e�[�u���̏����� */
        block_info_table.block_info[peer_aid] = (WBTBlockInfo *)OS_Alloc(sizeof(WBTBlockInfo));
#endif

        if (uc->target_bmp == 0)       /* ���ׂĂ̑���ǂ��烌�X�|���X�����������H */
        {
            (void)OS_SPrintf((char *)user_data, " %5d\n", bt_loop_count);

            /* ���[�U�[�f�[�^���M */
            if (FALSE == WBT_PutUserData(tbmp,  /* ���[�U�[�f�[�^�𑗂鑊��i�e�@�̂ݕ����j */
                                         user_data,     /* ���[�U�[�f�[�^�o�b�t�@ */
                                         WBT_SIZE_USER_DATA,    /* ���[�U�[�f�[�^�T�C�Y 12�ȉ� */
                                         bt_callback    /* �I�����R�[���o�b�N */
                ))
            {
                mfprintf(tc[2], "command invoke error %d\n", __MY_LINE__);
            }
        }
        break;
    case WBT_CMD_RES_USER_DATA:

        if (uc->target_bmp == 0)       /* ���ׂĂ̑���ǂ��烌�X�|���X�����������H */
        {
            /* �u���b�N���X�g�v�� */
            if (FALSE == WBT_GetBlockInfo(tbmp, /* �u���b�N���X�g�v�������鑊��i�e�@�̂ݕ����j */
                                          request_block_num /* �u���b�N���X�gNO */ ,
                                          &block_info_table,    /* �u���b�N���e�[�u�� */
                                          bt_callback   /* �I�����R�[���o�b�N */
                ))
            {
                mfprintf(tc[2], "command invoke error %d\n", __MY_LINE__);
            }
        }
        break;
    case WBT_CMD_RES_GET_BLOCKINFO:

        /* WBT_GetBlockInfo�I�� */

        mfprintf(tc[2], "blockinfo %d done\n", uc->get.block_id);       /* �擾�����u���b�N���X�g��ID */
        mfprintf(tc[2], " info id = %d\n", block_info_table.block_info[peer_aid]->id);  /* �u���b�NID */
        mfprintf(tc[2], " info block size = %d\n", block_info_table.block_info[peer_aid]->block_size);  /* �u���b�N�̃T�C�Y */
        mfprintf(tc[2], " info = %s\n", block_info_table.block_info[peer_aid]->user_id);        /* �u���b�N�̃��[�U��`��� */

#ifndef NOT_USE_ALLOC
        /* ��M�o�b�t�@�e�[�u���̏����� */
        recv_buf_table.recv_buf[peer_aid] =
            (u8 *)OS_Alloc((u32)block_info_table.block_info[peer_aid]->block_size);
        mfprintf(tc[2], "recv buf alloc %d\n", peer_aid);

        /* �p�P�b�g��M�ԍ��L�^�p�o�b�t�@�e�[�u���̏����� */
        recv_buf_packet_bmp_table.packet_bitmap[peer_aid] =
            (u32 *)
            OS_Alloc((u32)
                     WBT_CalcPacketbitmapSize(block_info_table.block_info[peer_aid]->block_size));

        mfprintf(tc[2], "recv pkt bmp size = %d\n",
                 WBT_CalcPacketbitmapSize(block_info_table.block_info[peer_aid]->block_size));
#endif

        if (uc->target_bmp == 0)       /* ���ׂĂ̑���ǂ��烌�X�|���X�����������H */
        {

            /* �u���b�N��M�v�� */
            if (FALSE == WBT_GetBlock(tbmp,     /* �u���b�N��M�v�������鑊��i�e�@�̂ݕ����j */
                                      block_info_table.block_info[peer_aid]->id /* �u���b�NID */ ,
                                      &recv_buf_table,  /* ��M�o�b�t�@�e�[�u�� */
                                      (u32)block_info_table.block_info[peer_aid]->block_size,   /* �u���b�N�T�C�Y */
                                      &recv_buf_packet_bmp_table,       /* �p�P�b�g��M�ԍ��L�^�p�o�b�t�@�e�[�u�� */
                                      bt_callback       /* �I�����R�[���o�b�N */
                ))
            {
                mfprintf(tc[2], "command invoke error %d\n", __MY_LINE__);
            }
            else
            {
                start_tick = OS_GetTick();      /* ���Ԍv���J�n */
            }

        }

        break;
    case WBT_CMD_RES_GET_BLOCK:
        /* WBT_GetBlock�I�� */

        mfprintf(tc[2], "get block %d done\n", uc->get.block_id);       /* ��M�����u���b�NID */
        mfprintf(tc[2], " time %d msec\n", OS_TicksToMilliSeconds(OS_GetTick() - start_tick));

        mfprintf(tc[2], " %s\n", &(recv_buf_table.recv_buf[peer_aid][4]));      /* ��M�����u���b�N�̒��g */
        {
            u32     offset;
            offset = *(u32 *)&(recv_buf_table.recv_buf[peer_aid][0]);
            mfprintf(tc[2], " %s\n", (char *)(&(recv_buf_table.recv_buf[peer_aid][offset])));
        }

#ifndef NOT_USE_ALLOC
        /* ��M�o�b�t�@�e�[�u���̉�� */
        mfprintf(tc[2], "recv buf free %d\n", peer_aid);
        OS_Free(recv_buf_table.recv_buf[peer_aid]);
        recv_buf_table.recv_buf[peer_aid] = NULL;

        /* �p�P�b�g��M�ԍ��L�^�p�o�b�t�@�e�[�u���̉�� */
        OS_Free(recv_buf_packet_bmp_table.packet_bitmap[peer_aid]);
        recv_buf_packet_bmp_table.packet_bitmap[peer_aid] = NULL;

        OS_Free(block_info_table.block_info[peer_aid]);
        block_info_table.block_info[peer_aid] = NULL;

        {
            mfprintf(tc[2], "info buf alloc %d\n", peer_aid);
            /* �u���b�N���e�[�u���̏����� */
            block_info_table.block_info[peer_aid] = (WBTBlockInfo *)OS_Alloc(sizeof(WBTBlockInfo));
        }
#endif

        if (uc->target_bmp == 0)
        {                              /* �v���������ׂĂ̑���ǂ��烌�X�|���X�����������H */

            request_block_num++;

            if (request_block_num < block_info_num)
            {

                /* �u���b�N���X�g�v�� */
                if (FALSE == WBT_GetBlockInfo(tbmp, request_block_num,  /* �u���b�N���X�gNO */
                                              &block_info_table,        /* �u���b�N���e�[�u�� */
                                              bt_callback       /* �I�����R�[���o�b�N */
                    ))
                {
                    mfprintf(tc[2], "command invoke error %d\n", __MY_LINE__);
                }
            }
            else
            {
                request_block_num = 0;

                bt_loop_count++;
                if (bt_loop_count > 99999)
                {
                    bt_loop_count = 0;
                }

                (void)OS_SPrintf((char *)user_data, " %05d\n", bt_loop_count);

                /* ���[�U�[�f�[�^���M */
                if (FALSE == WBT_PutUserData(tbmp,      /* ���[�U�[�f�[�^�𑗂鑊��i�e�@�̂ݕ����j */
                                             user_data, /* ���[�U�[�f�[�^�o�b�t�@ */
                                             WBT_SIZE_USER_DATA,        /* ���[�U�[�f�[�^�T�C�Y 12�ȉ� */
                                             bt_callback        /* �I�����R�[���o�b�N */
                    ))
                {
                    mfprintf(tc[2], "command invoke error %d\n", __MY_LINE__);
                }
            }
        }
        break;
    case WBT_CMD_REQ_NONE:
        mfprintf(tc[2], "WBT user none\n");
        break;
    case WBT_CMD_REQ_USER_DATA:
        mfprintf(tc[2], "get user data = %s\n", uc->user_data.data);
        break;
    case WBT_CMD_REQ_GET_BLOCK_DONE:
        mfprintf(tc[2], "get peer getblockdone %d done from %d\n", uc->blockdone.block_id,
                 peer_aid);
        break;
    case WBT_CMD_REQ_SYNC:
        mfprintf(tc[2], "get peer sync from %d\n", peer_aid);
        break;
    case WBT_CMD_RES_ERROR:
        mfprintf(tc[2], "get req error %d from %d\n", peer_aid, uc->result);
        break;
    case WBT_CMD_REQ_ERROR:
        mfprintf(tc[2], "get res error %d from %d\n", peer_aid, uc->result);
        break;
    case WBT_CMD_CANCEL:
        mfprintf(tc[2], "get canncel [%s] command from %d\n", command_str[uc->command], peer_aid);
        break;
    default:
        mfprintf(tc[2], "WBT callback unknown %d\n", uc->event);
        break;
    }
}
