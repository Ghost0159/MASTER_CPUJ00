/*---------------------------------------------------------------------------*
  Project:  NitroSDK - WM - demos - wmDEMOlib
  File:     wm_tool.c

  Copyright 2003-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: wm_tool.c,v $
  Revision 1.40  2007/02/20 00:28:08  kitase_hirotake
  indent source

  Revision 1.39  2006/03/07 01:01:20  ooe
  wm_lib_add_parent_listEx()を追加。

  Revision 1.38  2006/02/20 02:41:16  seiki_masashi
  WM_GAMEINFO_TYPE_OLD のサポートを廃止

  Revision 1.37  2006/01/18 02:12:40  kitase_hirotake
  do-indent

  Revision 1.36  2005/11/02 04:29:21  ooe
  scan時のチャンネル情報を更新するように修正。

  Revision 1.35  2005/06/07 05:45:56  seiki_masashi
  Key Sharing に関する特別処理を低減するための変更

  Revision 1.34  2005/03/17 06:02:13  ooe
  otherElement取得実験用コードを追加。

  Revision 1.33  2005/03/03 10:14:13  ooe
  save_parentInfoEx()で、gameInfoのアライメントがずれるバグを修正。

  Revision 1.32  2005/02/28 05:26:27  yosizaki
  do-indent.

  Revision 1.31  2005/02/17 08:08:44  ooe
  wm_lib_set_ggidFilterを追加。

  Revision 1.30  2005/02/17 00:16:33  ooe
  ggidFilterの判定を追加。

  Revision 1.29  2005/02/08 09:36:12  ooe
  copyright 年表記修正.

  Revision 1.28  2005/02/08 06:56:32  ooe
  ssidListの型をu16からu8に変更。

  Revision 1.27  2004/11/02 07:39:16  terui
  Bug fix in wm_lib_get_parent_gameinfo_gamename.

  Revision 1.26  2004/10/27 04:23:19  ooe
  WM_StartScanExコールバック引数の変更に対応。

  Revision 1.25  2004/10/26 08:25:56  ooe
  save_parentInfoEx()を追加。

  Revision 1.24  2004/10/18 04:17:46  terui
  ビーコン間隔、スキャン間隔をばらつかせる仕様を追加。

  Revision 1.23  2004/10/05 09:45:28  terui
  MACアドレスをu8型の配列に統一。

  Revision 1.22  2004/10/04 13:40:25  terui
  ゲームグループIDをu32型に統一。

  Revision 1.21  2004/10/01 00:20:46  ikedae
  コメント削除

  Revision 1.20  2004/09/30 09:31:52  seiki_masashi
  GGID に割り振られた正式な値を設定

  Revision 1.19  2004/08/30 05:36:24  sato_masaki
  add wm_lib_set_gameinfo function

  Revision 1.18  2004/08/18 09:07:53  miya
  modified WMGameInfo format

  Revision 1.17  2004/08/18 00:25:54  seiki_masashi
  recvBuf を u16* に変更

  Revision 1.16  2004/08/10 05:46:42  ooe
  find_counterを追加(Scan実験用)

  Revision 1.15  2004/08/03 23:44:21  sato_masaki
  small fix

  Revision 1.14  2004/08/03 12:00:12  sato_masaki
  WMbssDesc構造体経由で、接続処理を行う関数。wm_lib_connect_parent_via_bssdescを追加。

  Revision 1.13  2004/07/30 03:59:14  miya
  DCF並列のため

  Revision 1.12  2004/07/29 01:49:49  miya
  自動ビーコンスキャン制御をやめた

  Revision 1.11  2004/07/29 01:00:00  miya
  small fix

  Revision 1.10  2004/07/29 00:48:18  miya
  small fix

  Revision 1.9  2004/07/29 00:45:57  miya
  small fix

  Revision 1.8  2004/07/29 00:15:13  miya
  bssid(MACアドレス)指定ビーコンスキャンに対応

  Revision 1.7  2004/07/28 08:52:55  miya
  samll fix

  Revision 1.6  2004/07/28 08:34:46  miya
  add comment

  Revision 1.5  2004/07/28 04:28:53  miya
  add functions about ggid & tgid

  Revision 1.4  2004/07/28 03:54:07  ikedae
  libParam等を毎回ゼロクリア

  Revision 1.3  2004/07/28 02:24:05  miya
  add functions about UserGameInfo

  Revision 1.2  2004/07/27 09:12:43  sato_masaki
  small fix

  Revision 1.1  2004/07/23 15:29:17  terui
  整理して新規追加

  Revision 1.4  2004/07/23 14:28:23  terui
  ヘッダコメントを追加

  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#include <nitro.h>
#include <nitro/wm.h>
#include "wm_lib.h"
#include "ni.h"

#define WM_BEACON_PERIOD_DEFAULT    (200)       // Beacon間隔(ms)デフォルト値


static WM_lib_param libParam ATTRIBUTE_ALIGN(32);       // WM_lib用パラメータ構造体

static u8 libParam_wmbuf[WM_SYSTEM_BUF_SIZE] ATTRIBUTE_ALIGN(32);
static u8 libParam_statusBuf[WM_STATUS_BUF_SIZE] ATTRIBUTE_ALIGN(32);
static u8 libParam_parentParam[WM_PARENT_PARAM_SIZE] ATTRIBUTE_ALIGN(32);

static u8 libParam_dcfBuf[WM_DCF_MAX_SIZE * 2] ATTRIBUTE_ALIGN(32);

static u8 libParam_parentInfoBuf[WM_BSS_DESC_SIZE] ATTRIBUTE_ALIGN(32);
static u8 my_parentInfo[WM_NUM_PARENT_INFORMATIONS][WM_BSS_DESC_SIZE] ATTRIBUTE_ALIGN(32);

static CHILD_INFO child_info[WM_NUM_CHILD_INFORMATIONS] ATTRIBUTE_ALIGN(32);
static childInfo ACinfo;               // Childモードでの変数構造体

static WMkeySetBuf libParam_ksbuf ATTRIBUTE_ALIGN(32);  // Keysharingバッファ

static u16 ni_flag = 0;                // NIモード使用フラグ

// 親機情報格納用(4台分) 接続時に使用する
static u16 userGameInfoBuf[32] ATTRIBUTE_ALIGN(32);

static u8 user_name[WM_SIZE_USERNAME] ATTRIBUTE_ALIGN(4);
static u8 game_name[WM_SIZE_GAMENAME] ATTRIBUTE_ALIGN(4);

static int last_found_parent_no = -1;

int wm_lib_get_last_found_parent_no(void)
{
    return last_found_parent_no;
}

//--------------------------------------------------------------
// Scanで取得した親機情報の格納
void save_parentInfo(WMstartScanCallback *buf, WM_lib_param * param, childInfo * info)
{
    int     i = 0;

    // WM_lib_set_scan_channel_seek(FALSE);
    // wm_lib_set_scan_bssid(buf->macAddress[0],buf->macAddress[1], buf->macAddress[2]);

    while (i < info->found_parent_count)
    {
        if (buf->macAddress[0] == info->MacAdrList[i][0] &&
            buf->macAddress[1] == info->MacAdrList[i][1] &&
            buf->macAddress[2] == info->MacAdrList[i][2] &&
            buf->macAddress[3] == info->MacAdrList[i][3] &&
            buf->macAddress[4] == info->MacAdrList[i][4] &&
            buf->macAddress[5] == info->MacAdrList[i][5])
        {

            ++info->find_counter[i];   // 発見回数をインクリメント

            info->channelList[i] = buf->channel;        // チャンネル情報を更新
            info->gameInfoLength[i] = buf->gameInfoLength;
            info->gameInfoList[i] = buf->gameInfo;
            DC_InvalidateRange(info->parentInfo[i], WM_BSS_DESC_SIZE);  // キャッシュを無効化
            MI_DmaCopy16(WM_DMA_NO,    // DMA番号
                         param->parentInfoBuf,  // src
                         info->parentInfo[i],   // dest
                         WM_BSS_DESC_SIZE       // 転送サイズ
                );
            last_found_parent_no = i;
            return;
        }
        i++;
    }

    if (i >= WM_NUM_PARENT_INFORMATIONS)
    {
        return;                        /* 登録数制限を越えたとき */
    }

    // 指定GGIDかどうかのチェック
    if (param->ggidFilter != 0xffffffff)
    {
        if (buf->gameInfoLength == 0 || buf->gameInfo.ggid != param->ggidFilter)
            return;
    }

    info->found_parent_count = (u16)(i + 1);


    // 新規に発見した親機なので、データを格納する
    info->MacAdrList[i][0] = buf->macAddress[0];
    info->MacAdrList[i][1] = buf->macAddress[1];
    info->MacAdrList[i][2] = buf->macAddress[2];
    info->MacAdrList[i][3] = buf->macAddress[3];
    info->MacAdrList[i][4] = buf->macAddress[4];
    info->MacAdrList[i][5] = buf->macAddress[5];
    info->channelList[i] = buf->channel;
    info->ssidLength[i] = buf->ssidLength;

    MI_CpuCopy8(buf->ssid, info->ssidList[i], 32);
    //    for(j = 0; j < 16; j++ )
    //        info->ssidList[i][j]       = buf->ssid[j];
    info->gameInfoLength[i] = buf->gameInfoLength;
    info->gameInfoList[i] = buf->gameInfo;

    DC_InvalidateRange(info->parentInfo[i], WM_BSS_DESC_SIZE);  // キャッシュを無効化

    MI_DmaCopy16(WM_DMA_NO,            // DMA番号
                 param->parentInfoBuf, // src
                 info->parentInfo[i],  // dest
                 WM_BSS_DESC_SIZE      // 転送サイズ
        );
    last_found_parent_no = i;
}

//--------------------------------------------------------------
// Scanで取得した親機情報の格納
void save_parentInfoEx(WMstartScanExCallback *buf, WM_lib_param * param, childInfo * info)
{
    int     i, j, k;

    WMBssDesc *bssDesc;
    u16     bssDescLength;
    // WMOtherElements elems;   // otherElement取得実験用

    // 親機情報格納バッファのキャッシュを無効化
    DC_InvalidateRange(param->parentInfoBuf, WM_SIZE_SCAN_EX_BUF);

    // 今回見つけた親機の数だけループする
    for (k = 0; k < buf->bssDescCount; ++k)
    {
        // 親機情報の先頭アドレスをコールバックから取得
        bssDesc = buf->bssDesc[k];

        // otherElement取得実験
        //elems = WM_GetOtherElements( bssDesc );

        i = 0;
        while (1)
        {
            // bssDescの長さは16bit単位なので、バイト長に変換しておく。
            bssDescLength = (u16)(bssDesc->length * 2);

            // 今までに見つけていた親機かどうかのチェック
            if (i < info->found_parent_count)
            {
                if (bssDesc->bssid[0] == info->MacAdrList[i][0] &&
                    bssDesc->bssid[1] == info->MacAdrList[i][1] &&
                    bssDesc->bssid[2] == info->MacAdrList[i][2] &&
                    bssDesc->bssid[3] == info->MacAdrList[i][3] &&
                    bssDesc->bssid[4] == info->MacAdrList[i][4] &&
                    bssDesc->bssid[5] == info->MacAdrList[i][5])
                {
                    ++info->find_counter[i];    // 発見回数をインクリメント

                    info->channelList[i] = bssDesc->channel;    // チャンネル情報を更新
                    info->gameInfoLength[i] = bssDesc->gameInfoLength;

                    if (info->gameInfoLength[i] != 0)
                    {
                        MI_CpuCopy16((u16 *)&(bssDesc->gameInfo),
                                     (u16 *)&(info->gameInfoList[i]), info->gameInfoLength[i]);
                    }
                    MI_DmaCopy16(WM_DMA_NO,     // DMA番号
                                 bssDesc,       // src
                                 info->parentInfo[i],   // dest
                                 bssDescLength  // 転送サイズ
                        );

                    break;             // 次の親機情報チェックへ
                }
            }
            else
            {
                // 親機情報リストが全部埋まっていたら何もせず終了
                if (i == WM_NUM_PARENT_INFORMATIONS - 1)
                    break;

                // 指定GGIDかどうかのチェック
                if (param->ggidFilter != 0xffffffff)
                {
                    if (bssDesc->gameInfoLength == 0 || bssDesc->gameInfo.ggid != param->ggidFilter)
                        break;
                }

                if (i == info->found_parent_count)
                {
                    // 新規に見つけた親機なので、親機情報リストに追加する。
                    info->found_parent_count = (u16)(i + 1);

                    info->MacAdrList[i][0] = bssDesc->bssid[0];
                    info->MacAdrList[i][1] = bssDesc->bssid[1];
                    info->MacAdrList[i][2] = bssDesc->bssid[2];
                    info->MacAdrList[i][3] = bssDesc->bssid[3];
                    info->MacAdrList[i][4] = bssDesc->bssid[4];
                    info->MacAdrList[i][5] = bssDesc->bssid[5];

                    info->channelList[i] = bssDesc->channel;
                    info->ssidLength[i] = bssDesc->ssidLength;
                    for (j = 0; j < 16; j++)
                        info->ssidList[i][j] = bssDesc->ssid[j];
                    info->gameInfoLength[i] = bssDesc->gameInfoLength;

                    if (info->gameInfoLength[i] != 0)
                    {
                        MI_CpuCopy16((u16 *)&(bssDesc->gameInfo),
                                     (u16 *)&(info->gameInfoList[i]), info->gameInfoLength[i]);
                    }

                    MI_DmaCopy16(WM_DMA_NO,     // DMA番号
                                 bssDesc,       // src
                                 info->parentInfo[i],   // dest
                                 bssDescLength  // 転送サイズ
                        );

                    break;             // 次の親機情報チェックへ
                }
            }
            ++i;
        }
    }
}

void wm_lib_comm_init(void)
{
    int     i;

    WM_lib_set_auto(TRUE);

    MI_DmaClear32(WM_DMA_NO, &libParam, sizeof(libParam));
    MI_DmaClear32(WM_DMA_NO, libParam_statusBuf, WM_STATUS_BUF_SIZE);
    MI_DmaClear32(WM_DMA_NO, libParam_wmbuf, WM_SYSTEM_BUF_SIZE);
    MI_DmaClear32(WM_DMA_NO, libParam_parentParam, WM_PARENT_PARAM_SIZE);
    MI_DmaClear32(WM_DMA_NO, libParam_parentInfoBuf, WM_BSS_DESC_SIZE);
    MI_DmaClear32(WM_DMA_NO, my_parentInfo, sizeof(my_parentInfo));
    MI_DmaClear32(WM_DMA_NO, child_info, sizeof(child_info));
    MI_DmaClear32(WM_DMA_NO, &ACinfo, sizeof(ACinfo));
    MI_DmaClear32(WM_DMA_NO, &libParam_ksbuf, sizeof(libParam_ksbuf));

    libParam.statusBuf = (WMstatus *)&libParam_statusBuf;
    libParam.wmBuf = (void *)&libParam_wmbuf;
    libParam.parentParam = (WMpparam *)&libParam_parentParam;
    libParam.parentInfoBuf = (WMbssDesc *)&libParam_parentInfoBuf;

    libParam.dcfBufSize = WM_DCF_MAX_SIZE;
    libParam.dcfBuf = (WMdcfRecvBuf *)libParam_dcfBuf;

    libParam.wepMode = WM_WEPMODE_NO;  /* WM_WEPMODE_40 */

    libParam.keySharing = 0;
    libParam.contSend = 0;             // 1 -> 連続送信

    libParam.parentParam->KS_Flag = libParam.keySharing;
    libParam.parentParam->CS_Flag = libParam.contSend;


    /*
       OS_Printf("parentParam = %d %d\n",sizeof(WMpparam), WM_PARENT_PARAM_SIZE);
       OS_Printf("parentInfoBuf = %d %d\n",sizeof(WMbssDesc), WM_BSS_DESC_SIZE);
       OS_Printf("wmstatus = %d %d\n",sizeof(WMstatus), WM_STATUS_BUF_SIZE);
     */

    // 親機情報(子機がScan時に取得する情報)をセット
    libParam.parentParam->ggid = 0x003fff12;
    libParam.parentParam->tgid = 123;  // ランダム値を用いる予定
    libParam.parentParam->entryFlag = 1;        // 子機のエントリ許可
    libParam.parentParam->multiBootFlag = 0;    // マルチブート不許可

    libParam.parentParam->beaconPeriod = WM_GetDispersionBeaconPeriod();        // Beacon間隔(ms)デフォルト値
    libParam.parentParam->maxEntry = 15;


//  userGameInfoBuf[0] = 0x1111;        // test
//  userGameInfoBuf[1] = 0x2222;
//  userGameInfoBuf[2] = 0x3333;
//  userGameInfoBuf[3] = 0x4444;

    libParam.parentParam->userGameInfo = userGameInfoBuf;
    libParam.parentParam->userGameInfoLength = 32;

    libParam.sendBufSize = 0;
    libParam.recvBufSize = 0;

    for (i = 0; i < WM_NUM_CHILD_INFORMATIONS; i++)
    {
        child_info[i].valid = 0;
    }

    wm_lib_parent_found_count_reset();
    for (i = 0; i < WM_NUM_PARENT_INFORMATIONS; i++)
    {
        ACinfo.parentInfo[i] = (WMbssDesc *)&(my_parentInfo[i][0]);
        ACinfo.MacAdrList[i][0] = 0;
        ACinfo.MacAdrList[i][1] = 0;
        ACinfo.MacAdrList[i][2] = 0;
        ACinfo.MacAdrList[i][3] = 0;
        ACinfo.MacAdrList[i][4] = 0;
        ACinfo.MacAdrList[i][5] = 0;
    }
    ACinfo.my_aid = (u16)0;

    ni_flag = 0;                       // NI転送フラグオフ
}

void wm_lib_set_callback(WMcallbackFunc2 callback)
{
    libParam.callback = callback;
}


void wm_lib_set_parent_send_size(u16 size)
{
    libParam.parentParam->parentMaxSize = size; // 親機最大送信サイズ(Byte)
    //  libParam.sendBufSize =  size;                // バッファ長
    libParam.sendBufSize = WM_CalcParentSendBufSize(libParam);

}

void wm_lib_set_parent_recv_size_per_child(u16 size)
{
    libParam.parentParam->childMaxSize = size;  // 子機最大送信サイズ(Byte)
    libParam.recvBufSize = WM_CalcParentRecvBufSize(libParam);
}


void wm_lib_set_keySharing_mode(int flag)
{
    libParam.keySharing = (u16)flag;   // 削除予定
    libParam.parentParam->KS_Flag = (u16)flag;
    libParam.ksBuf = &libParam_ksbuf;
}


void wm_lib_set_contSend_mode(int flag)
{
    libParam.contSend = (u16)flag;     // 削除予定
    libParam.parentParam->CS_Flag = (u16)flag;
}


void wm_lib_set_multiboot_mode(int flag)
{
    libParam.parentParam->multiBootFlag = (u16)flag;
}

void wm_lib_set_parent_channel(u16 ch)
{
    libParam.parentParam->channel = ch;
}

void wm_lib_set_beacon_period(u16 period_ms)
{
    libParam.parentParam->beaconPeriod = period_ms;
}

void wm_lib_set_ni_mode(int flag)
{
    ni_flag = (u16)flag;               // NI転送フラグセット
    wm_lib_set_contSend_mode(1);       // 連続送信セット
}

#define WM_LIB_GGID_LO_MASK     (0x0000ffff)
#define WM_LIB_GGID_HI_MASK     (0xffff0000)
#define WM_LIB_GGID_HI_SHIFT    (16)

void wm_lib_set_ggid(u32 ggid)
{
    libParam.parentParam->ggid = ggid;
}

u32 wm_lib_get_ggid(void)
{
    return libParam.parentParam->ggid;
}

void wm_lib_set_tgid(u16 tgid)
{
    libParam.parentParam->tgid = tgid;
}

u16 wm_lib_get_tgid(void)
{
    return libParam.parentParam->tgid;
}

void wm_lib_set_scan_bssid(u16 bssid0, u16 bssid1, u16 bssid2)
{
    WM_lib_set_bssid(bssid2, bssid1, bssid0);
}

void wm_lib_set_gameinfo_username(char *name)
{
    int     i;
    char   *c = (char *)user_name;
    for (i = 0; i < WM_SIZE_USERNAME; i++)
    {
        if (*name == 0)
        {
            break;
        }
        *c++ = *name++;
    }
    libParam.uname = user_name;
}

void wm_lib_set_gameinfo_gamename(char *game)
{
    int     i;
    char   *c = (char *)game_name;
    for (i = 0; i < WM_SIZE_GAMENAME; i++)
    {
        if (*game == 0)
        {
            break;
        }
        *c++ = *game++;
    }
    libParam.gname = game_name;
}


void wm_lib_set_mode(u16 mode)
{
    libParam.mode = mode;
}

void wm_lib_set_sendbuffer(u8 *buf)
{
    libParam.sendBuf = (u16 *)buf;
}

void wm_lib_set_recvbuffer(u8 *buf)
{
    libParam.recvBuf = (u16 *)buf;
}

int wm_lib_get_recvbuffer_size(void)
{
    return (int)libParam.recvBufSize;
}

int wm_lib_get_sendbuffer_size(void)
{
    return (int)libParam.sendBufSize;
}

const char *wm_lib_get_wlib_version(void)
{
    return (const char *)(libParam.statusBuf->wlVersion);
}

void wm_lib_set_ggidFilter(u32 ggidFilter)
{
    libParam.ggidFilter = ggidFilter;
}


int wm_lib_start(void)
{
    int     errcode;

    errcode = WM_lib_Init(&libParam);  // WM_libの初期化

    if (ni_flag == 1)
    {                                  // NIブロック転送のスタート
        NI_Initialize((u16)((libParam.mode == WM_MODE_MP_PARENT) ? NI_MODE_PARENT : NI_MODE_CHILD),
                      &libParam);
    }

    return errcode;
}



BOOL wm_lib_get_own_macaddress(MACADDRESS * ma)
{
    int     i;
    (void)WM_ReadStatus(libParam.statusBuf);
    for (i = 0; i < 6; i++)
    {
        ma->addr[i] = libParam.statusBuf->MacAddress[i];
    }
    return TRUE;
}


/* 親機用関数 */
BOOL wm_lib_is_child_valid(int aid)
{
    if (child_info[aid].valid)
        return TRUE;
    else
        return FALSE;
}


void wm_lib_add_child_list(WMstartParentCallback *arg)
{
    int     i;
    child_info[arg->aid].valid = 1;
    for (i = 0; i < 6; i++)
    {
        child_info[arg->aid].macaddr.addr[i] = arg->macAddress[i];
    }
}

void wm_lib_delete_child_list(WMstartParentCallback *arg)
{
    child_info[arg->aid].valid = 0;
}

BOOL wm_lib_get_child_macaddress(int aid, MACADDRESS * ma)
{
    int     i;
    for (i = 0; i < 6; i++)
    {
        ma->addr[i] = child_info[aid].macaddr.addr[i];
    }
    return TRUE;
}

int wm_lib_set_gameinfo(void)
{
    // ユーザーネーム、ゲームネームのセット
    MI_DmaCopy16(WM_DMA_NO, libParam.uname,
                 &libParam.parentParam->userGameInfo[0], WM_SIZE_USERNAME);
    MI_DmaCopy16(WM_DMA_NO, libParam.gname,
                 &libParam.parentParam->userGameInfo[WM_SIZE_USERNAME / sizeof(u16)],
                 WM_SIZE_GAMENAME);
    return WM_lib_SetGameInfo(libParam.parentParam->userGameInfo,
                              libParam.parentParam->userGameInfoLength,
                              libParam.parentParam->ggid, libParam.parentParam->tgid);
}

/******************************/
/* 子機用関数 */

int wm_lib_get_my_aid(void)
{
    return ACinfo.my_aid;
}

void wm_lib_set_my_aid(int aid)
{
    ACinfo.my_aid = (u16)aid;
}

void wm_lib_add_parent_list(WMstartScanCallback *arg)
{
    save_parentInfo(arg, &libParam, &ACinfo);
}

void wm_lib_add_parent_listEx(WMStartScanExCallback *arg)
{
    save_parentInfoEx(arg, &libParam, &ACinfo);
}

void wm_lib_delete_parent_list(WMstartScanCallback *arg)
{
#pragma unused(arg)
    /* WM_STATECODE_PARENT_NOT_FOUND で呼ばれる */
    //wm_lib_set_scan_bssid(arg->macAddress[0],buf->macAddress[1], buf->macAddress[2]);  
    // WM_lib_set_scan_channel_seek(TRUE);
    // wm_lib_set_scan_bssid(0xffff,0xffff,0xffff);
}

int wm_lib_get_parent_gameinfo_channel(int id)
{
    return (int)(ACinfo.channelList[id]);
}

BOOL wm_lib_is_parent_gameinfo_valid(int id)
{
    if (ACinfo.gameInfoLength[id] != 0)
        return TRUE;
    else
        return FALSE;
}

int wm_lib_get_parent_gameinfo_parent_sendmaxsize(int id)
{
    return ACinfo.gameInfoList[id].parentMaxSize;
}

int wm_lib_get_parent_gameinfo_child_sendbufsize(int id)
{
    // return ACinfo.gameInfoList[id].childMaxSize;
    return WM_CalcChildSendBufSize((ACinfo.parentInfo[id]));
}

int wm_lib_get_parent_gameinfo_child_recvbufsize(int id)
{
    return WM_CalcChildRecvBufSize((ACinfo.parentInfo[id]));
}


const char *wm_lib_get_parent_gameinfo_username(int id)
{
    return (const char *)(ACinfo.gameInfoList[id].old_type.userName);
}

const char *wm_lib_get_parent_gameinfo_gamename(int id)
{
    return (const char *)(ACinfo.gameInfoList[id].old_type.gameName);
}


u32 wm_lib_get_parent_gameinfo_ggid(int id)
{
    return (ACinfo.gameInfoList[id].ggid);
}

u16 wm_lib_get_parent_gameinfo_tgid(int id)
{
    return (ACinfo.gameInfoList[id].tgid);
}

u16    *wm_lib_get_parent_gameinfo_usergameinfo(int id)
{
    return (ACinfo.gameInfoList[id].userGameInfo);
}

u16 wm_lib_get_parent_gameinfo_usergameinfosize(int id)
{
    return (ACinfo.gameInfoList[id].userGameInfoLength);
}


BOOL wm_lib_get_parent_macaddress(int id, MACADDRESS * ma)
{
    int     i;
    for (i = 0; i < 6; i++)
    {
        ma->addr[i] = ACinfo.MacAdrList[id][i];
    }
    return TRUE;
}

void wm_lib_parent_found_count_reset(void)
{
    ACinfo.found_parent_count = 0;
}

int wm_lib_get_parent_found_count(void)
{
    return ACinfo.found_parent_count;
}

int wm_lib_get_keyset(WMkeySet *keyset)
{
    return WM_GetKeySet(libParam.ksBuf, keyset);
}

int wm_lib_connect_parent(int parent_no)
{
    WMbssDesc *pInfo = (ACinfo.parentInfo[parent_no]);
    libParam.recvBufSize = (u16)wm_lib_get_parent_gameinfo_child_recvbufsize(parent_no);
    libParam.sendBufSize = (u16)wm_lib_get_parent_gameinfo_child_sendbufsize(parent_no);
    return WM_lib_ConnectToParent(pInfo);
}


//  get pointer to  WMbssDesc structure
WMbssDesc *wm_get_parent_bssdesc(int id, WMbssDesc *bssDescp)
{

    if (bssDescp != NULL)
    {
        MI_CpuCopy16(ACinfo.parentInfo[id], bssDescp, sizeof(WMbssDesc));
    }
    return ACinfo.parentInfo[id];

}

int wm_lib_connect_parent_via_bssdesc(WMbssDesc *bssDescp)
{
    if (bssDescp == NULL)
        return WM_ERRCODE_FAILED;

    libParam.recvBufSize = (u16)WM_CalcChildRecvBufSize(bssDescp);
    libParam.sendBufSize = (u16)WM_CalcChildSendBufSize(bssDescp);
    return WM_lib_ConnectToParent(bssDescp);
}

/*---------------------------------------------------------------------------*
  End of file
 *---------------------------------------------------------------------------*/
