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
  Port受信コールバックの使用とプリント文をNITRO画面上に表示するように変更

  Revision 1.12  2004/10/04 08:40:35  miya
  add WBT_CancelCurrentCommand

  Revision 1.11  2004/10/04 05:49:53  miya
  change transfer size according to the programing guideline

  Revision 1.10  2004/09/13 09:12:22  miya
  OS_TPrintfを使用するように変更

  Revision 1.9  2004/09/13 06:39:43  miya
  マクロ追加

  Revision 1.8  2004/09/01 04:00:15  miya
  PutBlock,RequestRegisterBlock,ResponseRegisterBlock,Doneコマンド削除

  Revision 1.7  2004/09/01 00:16:56  miya
  ユーザーデータのサイズ変更

  Revision 1.6  2004/08/28 11:26:56  miya
  WBT_PutUserData関数追加

  Revision 1.5  2004/08/27 07:08:27  miya
  名前などの変更

  Revision 1.4  2004/08/26 23:54:28  miya
  名前変更など

  Revision 1.3  2004/08/24 02:41:57  miya
  コメント追加

  Revision 1.2  2004/08/23 12:39:40  miya
  UI的に問題になりそうなところの変更

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

/* 子機用受信ブロック制御データ */
static int request_block_num;
static int block_info_num = 0;
static OSTick start_tick;
static int bt_loop_count = 0;
static int bt_running = 0;

/* 子機用受信用バッファ諸々 */
static WBTBlockInfoTable block_info_table;
static WBTRecvBufTable recv_buf_table;
static WBTPacketBitmapTable recv_buf_packet_bmp_table;
static WBTAidBitmap tbmp = 1;          /* リクエスト要求相手（親） */

/* 子機用ユーザーデータ送信バッファ */
static u8 user_data[WBT_SIZE_USER_DATA];

#ifdef NOT_USE_ALLOC
WBTBlockInfo block_info[WBT_NUM_OF_AID][NUM_OF_BT_LIST];
u8      recv_buf[WBT_NUM_OF_AID][BT_DATA_SIZE];
u32    
    recv_pkt_bmp_buf[WBT_NUM_OF_AID][WBT_PACKET_BITMAP_SIZE(BT_DATA_SIZE, BT_PARENT_PACKET_SIZE)];
#endif

/* 親機用ブロックデータ */
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


/* 親機用ブロック登録関数 */
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

        (void)WBT_RegisterBlock(&(bt_list[i]), (u32)(10000 + i) /* id 1000以下は不可 */ ,
                                user_char_id[i], &(bt_data[i][0]), BT_DATA_SIZE, 0);
    }
}


/* 子機用ブロック転送終了関数 */
void bt_stop(void)
{
    bt_running = 0;
    (void)WBT_CancelCurrentCommand(0xffff);
}


/* 子機用ブロック転送開始関数 */
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
        /* 子機用受信用バッファ諸々の初期化 */
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

    (void)WBT_RequestSync(tbmp,        /* Sync要求をする相手（親機のみ複数可） */
                          bt_callback  /* 終了時コールバック */
        );
}


/* ブロック転送汎用コールバック関数 */
void bt_callback(void *arg)
{

    WBTCommand *uc = (WBTCommand *)arg;
    int     peer_aid = WBT_AidbitmapToAid(uc->peer_bmp);        /* 通信相手局のAID */
    // mfprintf(tc[2],"aid = %d\n", peer_aid);

    switch (uc->event)
    {
    case WBT_CMD_RES_SYNC:
        /* WBT_RequestSync終了 */
        request_block_num = 0;
        block_info_num = uc->sync.num_of_list;  /* 相手の持っているブロック数 */
        mfprintf(tc[2], "blockinfo num = %d my_packet_size = %d peer_packet_size = %d\n", block_info_num, uc->sync.my_packet_size,      /* 相手の送信データサイズ */
                 uc->sync.peer_packet_size      /* こちらの送信データサイズ */
            );
#ifndef NOT_USE_ALLOC
        mfprintf(tc[2], "info buf alloc %d\n", peer_aid);
        /* ブロック情報テーブルの初期化 */
        block_info_table.block_info[peer_aid] = (WBTBlockInfo *)OS_Alloc(sizeof(WBTBlockInfo));
#endif

        if (uc->target_bmp == 0)       /* すべての相手局からレスポンスがあったか？ */
        {
            (void)OS_SPrintf((char *)user_data, " %5d\n", bt_loop_count);

            /* ユーザーデータ送信 */
            if (FALSE == WBT_PutUserData(tbmp,  /* ユーザーデータを送る相手（親機のみ複数可） */
                                         user_data,     /* ユーザーデータバッファ */
                                         WBT_SIZE_USER_DATA,    /* ユーザーデータサイズ 12以下 */
                                         bt_callback    /* 終了時コールバック */
                ))
            {
                mfprintf(tc[2], "command invoke error %d\n", __MY_LINE__);
            }
        }
        break;
    case WBT_CMD_RES_USER_DATA:

        if (uc->target_bmp == 0)       /* すべての相手局からレスポンスがあったか？ */
        {
            /* ブロックリスト要求 */
            if (FALSE == WBT_GetBlockInfo(tbmp, /* ブロックリスト要求をする相手（親機のみ複数可） */
                                          request_block_num /* ブロックリストNO */ ,
                                          &block_info_table,    /* ブロック情報テーブル */
                                          bt_callback   /* 終了時コールバック */
                ))
            {
                mfprintf(tc[2], "command invoke error %d\n", __MY_LINE__);
            }
        }
        break;
    case WBT_CMD_RES_GET_BLOCKINFO:

        /* WBT_GetBlockInfo終了 */

        mfprintf(tc[2], "blockinfo %d done\n", uc->get.block_id);       /* 取得したブロックリストのID */
        mfprintf(tc[2], " info id = %d\n", block_info_table.block_info[peer_aid]->id);  /* ブロックID */
        mfprintf(tc[2], " info block size = %d\n", block_info_table.block_info[peer_aid]->block_size);  /* ブロックのサイズ */
        mfprintf(tc[2], " info = %s\n", block_info_table.block_info[peer_aid]->user_id);        /* ブロックのユーザ定義情報 */

#ifndef NOT_USE_ALLOC
        /* 受信バッファテーブルの初期化 */
        recv_buf_table.recv_buf[peer_aid] =
            (u8 *)OS_Alloc((u32)block_info_table.block_info[peer_aid]->block_size);
        mfprintf(tc[2], "recv buf alloc %d\n", peer_aid);

        /* パケット受信番号記録用バッファテーブルの初期化 */
        recv_buf_packet_bmp_table.packet_bitmap[peer_aid] =
            (u32 *)
            OS_Alloc((u32)
                     WBT_CalcPacketbitmapSize(block_info_table.block_info[peer_aid]->block_size));

        mfprintf(tc[2], "recv pkt bmp size = %d\n",
                 WBT_CalcPacketbitmapSize(block_info_table.block_info[peer_aid]->block_size));
#endif

        if (uc->target_bmp == 0)       /* すべての相手局からレスポンスがあったか？ */
        {

            /* ブロック受信要求 */
            if (FALSE == WBT_GetBlock(tbmp,     /* ブロック受信要求をする相手（親機のみ複数可） */
                                      block_info_table.block_info[peer_aid]->id /* ブロックID */ ,
                                      &recv_buf_table,  /* 受信バッファテーブル */
                                      (u32)block_info_table.block_info[peer_aid]->block_size,   /* ブロックサイズ */
                                      &recv_buf_packet_bmp_table,       /* パケット受信番号記録用バッファテーブル */
                                      bt_callback       /* 終了時コールバック */
                ))
            {
                mfprintf(tc[2], "command invoke error %d\n", __MY_LINE__);
            }
            else
            {
                start_tick = OS_GetTick();      /* 時間計測開始 */
            }

        }

        break;
    case WBT_CMD_RES_GET_BLOCK:
        /* WBT_GetBlock終了 */

        mfprintf(tc[2], "get block %d done\n", uc->get.block_id);       /* 受信したブロックID */
        mfprintf(tc[2], " time %d msec\n", OS_TicksToMilliSeconds(OS_GetTick() - start_tick));

        mfprintf(tc[2], " %s\n", &(recv_buf_table.recv_buf[peer_aid][4]));      /* 受信したブロックの中身 */
        {
            u32     offset;
            offset = *(u32 *)&(recv_buf_table.recv_buf[peer_aid][0]);
            mfprintf(tc[2], " %s\n", (char *)(&(recv_buf_table.recv_buf[peer_aid][offset])));
        }

#ifndef NOT_USE_ALLOC
        /* 受信バッファテーブルの解放 */
        mfprintf(tc[2], "recv buf free %d\n", peer_aid);
        OS_Free(recv_buf_table.recv_buf[peer_aid]);
        recv_buf_table.recv_buf[peer_aid] = NULL;

        /* パケット受信番号記録用バッファテーブルの解放 */
        OS_Free(recv_buf_packet_bmp_table.packet_bitmap[peer_aid]);
        recv_buf_packet_bmp_table.packet_bitmap[peer_aid] = NULL;

        OS_Free(block_info_table.block_info[peer_aid]);
        block_info_table.block_info[peer_aid] = NULL;

        {
            mfprintf(tc[2], "info buf alloc %d\n", peer_aid);
            /* ブロック情報テーブルの初期化 */
            block_info_table.block_info[peer_aid] = (WBTBlockInfo *)OS_Alloc(sizeof(WBTBlockInfo));
        }
#endif

        if (uc->target_bmp == 0)
        {                              /* 要求したすべての相手局からレスポンスがあったか？ */

            request_block_num++;

            if (request_block_num < block_info_num)
            {

                /* ブロックリスト要求 */
                if (FALSE == WBT_GetBlockInfo(tbmp, request_block_num,  /* ブロックリストNO */
                                              &block_info_table,        /* ブロック情報テーブル */
                                              bt_callback       /* 終了時コールバック */
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

                /* ユーザーデータ送信 */
                if (FALSE == WBT_PutUserData(tbmp,      /* ユーザーデータを送る相手（親機のみ複数可） */
                                             user_data, /* ユーザーデータバッファ */
                                             WBT_SIZE_USER_DATA,        /* ユーザーデータサイズ 12以下 */
                                             bt_callback        /* 終了時コールバック */
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
