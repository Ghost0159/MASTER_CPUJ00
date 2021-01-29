/*---------------------------------------------------------------------------*
  Project:  NitroSDK - WM - demos - wmDEMOlib
  File:     ni.c

  Copyright 2003-2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: ni.c,v $
  Revision 1.8  2006/01/18 02:11:30  kitase_hirotake
  do-indent

  Revision 1.7  2005/06/27 11:10:11  yosizaki
  add comment about DC_WaitWriteBufferEmpty().

  Revision 1.6  2005/02/28 05:26:27  yosizaki
  do-indent.

  Revision 1.5  2005/02/18 13:47:59  seiki_masashi
  warning 対策

  Revision 1.4  2004/10/04 13:40:25  terui
  ゲームグループIDをu32型に統一。

  Revision 1.3  2004/07/28 04:28:53  miya
  add functions about ggid & tgid

  Revision 1.2  2004/07/27 09:02:03  ikedae
  不要なコメントの削除

  Revision 1.1  2004/07/23 15:29:17  terui
  整理して新規追加

  Revision 1.3  2004/07/23 14:47:34  terui
  インクルードヘッダの記述を変更

  Revision 1.2  2004/07/20 12:46:15  terui
  インクルードするヘッダを追加。

  Revision 1.1  2004/07/20 11:26:09  terui
  wm_lib切り離しに伴う改造

  Revision 1.2  2004/07/11 23:54:51  terui
  nginf.filenum u32 --> u16

  Revision 1.1  2004/07/08 10:08:07  terui
  WM7改造開始に伴う追加

  Revision 1.4  2004/07/02 04:56:06  ikedae
  (none)

  Revision 1.3  2004/06/30 05:57:42  ikedae
  ヘッダ作成　MAX_BLOCK変更


  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#include <nitro.h>
#include <string.h>

#include    <nitro/wm.h>
#include    "wm_lib.h"
#include    "ni.h"

static int NI_AnnounceFiles();
static void NI_ParentCallback(u16 type, void *arg);
static void NI_ChildCallback(u16 type, void *arg);

static void NI_ProceedParentCB1(void *arg, int i);
static void NI_ProceedChildCB(void *arg);

#define MY_ROUND(n, a)     (((u32) (n) + (a) - 1) & ~((a) - 1))

static u16 ni_mode;

static NI_STATUS nst;
static NI_INFO ninf;
static NI_GAME_INFO nginf ATTRIBUTE_ALIGN(32);

static NI_CALLBACK ni_callback;

#define MAX_BLOCK    8192              // max 8192 * 500 = 3.9Mbyte
static u8 recvflag[MAX_BLOCK / 8];

static WM_lib_param *libParamPtr;      // WM_lib用パラメータ構造体
static WMcallbackFunc2 stubCallback;

static inline u16 max(u16 a, u16 b)
{
    return (a > b) ? a : b;
}

static BOOL checkRecvFlag(int x)
{
    int     pos = x / 8;
    int     bit = x % 8;
    SDK_ASSERT(x < MAX_BLOCK);

    if (recvflag[pos] & (1 << bit))
    {
        return TRUE;
    }
    return FALSE;
}

static void setRecvFlag(int x)
{
    int     pos = x / 8;
    int     bit = x % 8;
    SDK_ASSERT(x < MAX_BLOCK);

    recvflag[pos] |= (1 << bit);
}

static u16 cksum(u16 *buf, int length)
{
    u32     sum;
    int     nwords = length >> 1;
    for (sum = 0; nwords > 0; nwords--)
        sum += *buf++;

    sum = (sum >> 16) + (sum & 0xffff);
    sum += (sum >> 16);
    return (u16)(sum ^ 0xffff);
}

void NI_Initialize(u16 mode, WM_lib_param * lptr)
{
    OSIntrMode bak;
    int     i;

    bak = OS_DisableInterrupts();
    SDK_ASSERT(mode == NI_MODE_PARENT || mode == NI_MODE_CHILD);
    ni_mode = mode;
    libParamPtr = lptr;

    stubCallback = libParamPtr->callback;
    libParamPtr->callback = (mode == NI_MODE_PARENT) ? NI_ParentCallback : NI_ChildCallback;
    MI_CpuClear8(&nst, sizeof(nst));
    MI_CpuClear8(&ninf, sizeof(ninf));
    MI_CpuClear8(&nginf, sizeof(nginf));

    for (i = 0; i < NI_MAX_CHILD; i++)
    {
        nst.fileid_of_child[i] = -1;
    }
    MI_CpuClear8(recvflag, sizeof(recvflag));
    ni_callback = NULL;

    (void)OS_RestoreInterrupts(bak);

}

void NI_GetStatus(NI_STATUS * stat)
{
    MI_CpuCopy8(&nst, stat, sizeof(nst));
}

void NI_End(void)
{
}

int NI_RegisterFile(char *filename, BOOL compress_flag, u8 *addr, u32 size)
{
    int     errcode;

    if (ninf.filenum + 1 >= NI_MAX_FILE)
    {
        return WM_ERRCODE_FAILED;
    }
    SDK_ASSERT(strlen(filename) < NI_FILENAME_SIZE);
    (void)strncpy(ninf.fileinfo[ninf.filenum].filename, filename, NI_FILENAME_SIZE);
    ninf.fileinfo[ninf.filenum].addr = addr;
    ninf.fileinfo[ninf.filenum].filesize = size;
    ninf.fileinfo[ninf.filenum].compress_flag = compress_flag;

    (void)strncpy(nginf.fileinfo[nginf.filenum].filename, filename, NI_FILENAME_SIZE);

    nginf.game_info_type = NI_GAME_INFO_TYPE_TEXT;      // 
    nginf.filenum = (u16)(++ninf.filenum);

    errcode = WM_lib_SetGameInfo((u16 *)&nginf, sizeof(nginf),
                                 libParamPtr->parentParam->ggid, libParamPtr->parentParam->tgid);

    return errcode;
}

void NI_GetInfo(NI_INFO * info)
{
    MI_CpuCopy8(&ninf, info, sizeof(ninf));
}

int NI_RequestFile(u8 fileid, u8 *addr, u32 size, NI_CALLBACK callback)
{
    OSIntrMode bak;
    int     errcode;

    NI_Header *hd = (NI_Header *) libParamPtr->sendBuf;
    nst.fileid = fileid;
    nst.total_length = size;
    nst.data_addr = addr;
    bak = OS_DisableInterrupts();
    ni_callback = callback;
    (void)OS_RestoreInterrupts(bak);
    MI_CpuClear8(hd, NI_HEADER_SIZE);

    hd->type = NI_TYPE_REQUEST;
    hd->fileid = fileid;
    hd->length = 0;
    hd->cksum = 0;
    hd->cksum = cksum(libParamPtr->sendBuf, NI_HEADER_SIZE);

    DC_FlushRange(libParamPtr->sendBuf, MY_ROUND(NI_HEADER_SIZE, 32));
    /* PXI操作でIOレジスタへアクセスするのでキャッシュの Wait は不要 */
    // DC_WaitWriteBufferEmpty();
    errcode = WM_lib_SetMPData(libParamPtr->sendBuf, NI_HEADER_SIZE);
    return errcode;
}


int NI_Proceed(void)
{
    static OSTick nexttick = 0;
    OSTick  currtick;
    u32     datalen;
    u16     sendlen;
    int     errcode = WM_ERRCODE_SUCCESS;
    int     i;

#define ANNOUNCE_INTERVAL    1000      // xxx msecに一回ファイル名をアナウンス

    currtick = OS_GetTick();
    if (currtick > nexttick)
    {
        nexttick = currtick + OS_MilliSecondsToTicks(ANNOUNCE_INTERVAL);
        if (ninf.filenum)
        {
            return NI_AnnounceFiles();
        }
    }

    for (i = 0; i < NI_MAX_FILE; i++)
    {
        nst.cur_id = (u8)((nst.cur_id + 1) % NI_MAX_FILE);
        if (nst.busyflag[nst.cur_id])
        {
            break;
        }
    }
    if (i == NI_MAX_FILE)              // 送信対象のファイルがない
    {
        return errcode;
    }
    if (WM_lib_CheckMPSend() == 1)
    {
        NI_Header *hd = (NI_Header *) libParamPtr->sendBuf;
        u8     *databuf = ((u8 *)libParamPtr->sendBuf) + NI_HEADER_SIZE;
        u32     remain;

        remain = ninf.fileinfo[nst.cur_id].filesize - (nst.cur_block[nst.cur_id] * NI_BUFSIZE);
        datalen = ((remain >= NI_BUFSIZE) ? NI_BUFSIZE : remain);
        MI_CpuCopy8(&ninf.fileinfo[nst.cur_id].addr[nst.cur_block[nst.cur_id] * NI_BUFSIZE],
                    databuf, datalen);
        hd->type = NI_TYPE_DATA;
        hd->fileid = nst.cur_id;
        hd->seqno = nst.cur_block[nst.cur_id];
        hd->length = (u16)datalen;
        sendlen = (u16)(NI_HEADER_SIZE + datalen);
        hd->cksum = 0;
        hd->cksum = cksum(libParamPtr->sendBuf, sendlen);

        DC_FlushRange(libParamPtr->sendBuf, MY_ROUND(sendlen, 32));
        /* PXI操作でIOレジスタへアクセスするのでキャッシュの Wait は不要 */
        // DC_WaitWriteBufferEmpty();
        errcode = WM_lib_SetMPData(libParamPtr->sendBuf, sendlen);
    }
    return errcode;
}

static void NI_ProceedParentCB1(void *arg, int child)
{
    int     i;
    NI_Header *hdp = (NI_Header *) arg;

    if (cksum((u16 *)hdp, (s32)(hdp->length + NI_HEADER_SIZE)))
    {
        nst.cksum_err++;
        return;
    }

    switch (hdp->type)
    {
    case NI_TYPE_REQUEST:
        if (nst.fileid_of_child[child] != hdp->fileid)
        {
            nst.num_of_children++;
        }
        SDK_ASSERT(hdp->fileid < NI_MAX_FILE);
        nst.busyflag[hdp->fileid] = TRUE;
        nst.cur_block[hdp->fileid] = 0;
        nst.fileid_of_child[child] = (s8)hdp->fileid;
        break;

    case NI_TYPE_CONTINUE:
// 複数のchildからきたnextSendのうち最大のものを送信対象とする
        SDK_ASSERT(hdp->fileid < NI_MAX_FILE);
        SDK_ASSERT(nst.busyflag[hdp->fileid]);
        nst.next_send[hdp->fileid] = max(hdp->next_block, nst.next_send[hdp->fileid]);
        break;
    case NI_TYPE_STOPREQUEST:
        SDK_ASSERT(hdp->fileid < NI_MAX_FILE);
        if (nst.fileid_of_child[child] != -1)
        {
            nst.num_of_children--;
            nst.fileid_of_child[child] = -1;
        }

        // もう誰もそのファイルを使っていないかをチェックする
        for (i = 0; i < NI_MAX_CHILD; i++)
        {
            if (nst.fileid_of_child[i] == hdp->fileid)
            {
                break;
            }
        }
        if (i == NI_MAX_CHILD)
        {
            nst.busyflag[hdp->fileid] = FALSE;
        }

        break;
    }
}


//-----------------------------------------------------------------------------

static void NI_ProceedChildCB(void *arg)
{
    u16     i;
    u32     block_num;
    int     errcode = WM_ERRCODE_SUCCESS;
    WMmpRecvBuf *bufp = (WMmpRecvBuf *)arg;
    NI_Header *p2c;
    u8     *databuf;
    NI_Header *c2p;

    DC_InvalidateRange(bufp, MY_ROUND(libParamPtr->recvBufSize, 32));

    p2c = (NI_Header *) (bufp->data);
    databuf = ((u8 *)bufp->data) + NI_HEADER_SIZE;
    c2p = (NI_Header *) libParamPtr->sendBuf;

    if (cksum((u16 *)p2c, (s32)(p2c->length + NI_HEADER_SIZE)))
    {
        nst.cksum_err++;
        return;
    }

    switch (p2c->type)
    {
    case NI_TYPE_FILEINFO:
        MI_CpuCopy8(((u8 *)bufp->data + NI_HEADER_SIZE), &ninf, sizeof(ninf));
        return;

    case NI_TYPE_DATA:                // 下の方で処理
        break;

    default:
        return;
    }

    c2p->type = NI_TYPE_CONTINUE;

    block_num = (nst.total_length + NI_BUFSIZE - 1) / NI_BUFSIZE;
    SDK_ASSERT(block_num < MAX_BLOCK);
    if (0 <= p2c->seqno && p2c->seqno < block_num)
    {
        if (checkRecvFlag(p2c->seqno) == FALSE)
        {
            nst.got_block++;
        }
        setRecvFlag(p2c->seqno);
        MI_CpuCopy8(databuf, &nst.data_addr[NI_BUFSIZE * p2c->seqno], p2c->length);
    }
// まだ受信していない最小のブロック番号を設定する
    c2p->next_block = 0;
    for (i = 0; i < block_num; i++)
    {
        if (checkRecvFlag(i) == FALSE)
        {
            c2p->next_block = i;
            break;
        }
    }
    if (i == block_num && ni_callback)
    {
        MI_CpuClear8(c2p, NI_HEADER_SIZE);

        c2p->type = NI_TYPE_STOPREQUEST;
        c2p->fileid = nst.fileid;

        if (ni_callback)
        {
            (*ni_callback) (0);
            ni_callback = NULL;
        }
    }

    if (WM_lib_CheckMPSend() == 1)
    {
        c2p->length = 0;
        c2p->cksum = 0;
        c2p->cksum = cksum(libParamPtr->sendBuf, NI_HEADER_SIZE);
        DC_FlushRange(libParamPtr->sendBuf, MY_ROUND(NI_HEADER_SIZE, 32));
        /* PXI操作でIOレジスタへアクセスするのでキャッシュの Wait は不要 */
        // DC_WaitWriteBufferEmpty();
        errcode = WM_lib_SetMPData(libParamPtr->sendBuf, NI_HEADER_SIZE);
    }
    return;
}

static int NI_AnnounceFiles()
{
    int     errcode = WM_ERRCODE_SUCCESS;
    if (ninf.filenum && WM_lib_CheckMPSend() == 1)
    {
        u16     sendlen;
        NI_Header *hd = (NI_Header *) libParamPtr->sendBuf;

        MI_CpuCopy8(&ninf, ((u8 *)libParamPtr->sendBuf) + NI_HEADER_SIZE, sizeof(ninf));
        hd->type = NI_TYPE_FILEINFO;
        hd->length = sizeof(ninf);
        hd->cksum = 0;
        hd->cksum = cksum(libParamPtr->sendBuf, sizeof(ninf));
        sendlen = NI_HEADER_SIZE + sizeof(ninf);
        DC_FlushRange(libParamPtr->sendBuf, MY_ROUND(sendlen, 32));
        /* PXI操作でIOレジスタへアクセスするのでキャッシュの Wait は不要 */
        // DC_WaitWriteBufferEmpty();
        errcode = WM_lib_SetMPData(libParamPtr->sendBuf, sendlen);
    }
    return errcode;
}

static void NI_ParentCallback(u16 type, void *arg)
{
    switch (type)
    {
    case WM_TYPE_MP_PARENT_RECV:
        {
            u16     i;
            WMmpRecvHeader *mpHeader;
            WMmpRecvData *datap;

            for (i = 0; i < NI_MAX_FILE; i++)
            {
                nst.next_send[i] = 0;
            }

            // 引数argに受信バッファのポインタが入っている
            mpHeader = (WMmpRecvHeader *)arg;

            DC_InvalidateRange(mpHeader, libParamPtr->recvBufSize);

            // 各子機からの受信データを表示
            for (i = 1; i < 16; ++i)
            {
                // AID==i の子機データの先頭アドレスを取得
                datap = WM_ReadMPData(mpHeader, i);

                // AID==i の子機データが存在した場合
                if (datap != NULL)
                {
                    if (datap->length == 0xffff)
                    {
                    }
                    else if (datap->length != 0)
                    {
                        NI_ProceedParentCB1(datap->cdata, i - 1);
                    }
                }
            }

            for (i = 0; i < NI_MAX_FILE; i++)
            {
                if (nst.busyflag[i])
                {
                    if (nst.next_send[i] <= nst.cur_block[i] &&
                        nst.cur_block[i] <= nst.next_send[i] + NI_SEND_THRESHOLD)
                    {
                        nst.cur_block[i]++;
                    }
                    else
                    {
                        nst.cur_block[i] = nst.next_send[i];
                    }
                }
            }
        }
        (void)NI_Proceed();            // メインループと同じ

        break;
    default:
        break;
    }
    stubCallback(type, arg);
}

static void NI_ChildCallback(u16 type, void *arg)
{
    switch (type)
    {
        //-----------------------------------------------------------------
        // MPデータの受信
    case WM_TYPE_MP_CHILD_RECV:
        (void)NI_ProceedChildCB(arg);
        break;
    default:
        break;
    }
    stubCallback(type, arg);
}

/*---------------------------------------------------------------------------*
  End of file
 *---------------------------------------------------------------------------*/
