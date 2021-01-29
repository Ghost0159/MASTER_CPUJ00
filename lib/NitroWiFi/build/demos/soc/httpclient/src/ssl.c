/*---------------------------------------------------------------------------*
  Project:  NitroWiFi - SOC - demos - httpclient
  File:     ssl.c

  Copyright 2005,2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: ssl.c,v $
  Revision 1.2  2006/03/10 09:22:43  kitase_hirotake
  INDENT SOURCE

  Revision 1.1  2005/12/22 04:34:06  adachi_hiroaki
  HTTPクライアントのデモをSOCで置き換え

  $NoKeywords$
 *---------------------------------------------------------------------------*/
#include <nitro.h>
#include "ssl.h"

static SOCSslConnection     g_sslCon;
static SOCSslConnection*    g_psslCon = NULL;

/* 認証局情報 */
static char ca_name[] = "Test CA, JP, Kyoto, Nintendo Co.,Ltd.";

static unsigned char    ca_modulo[] = {
    0xc5,0xfa,0x5d,0x50,0x46,0xc7,0x76,0xde,
    0x08,0x73,0x13,0x48,0xff,0xad,0x8b,0x84,
    0x7e,0xdf,0x75,0x93,0xf6,0x02,0xc4,0x65,
    0x97,0xef,0x17,0x51,0x8a,0x36,0xe1,0xe5,
    0x97,0xaa,0x26,0x50,0xf0,0x1c,0x80,0x1b,
    0x8d,0x0a,0xcf,0x24,0x51,0x71,0x40,0xc0,
    0xd6,0x6b,0x98,0x90,0xb7,0xb2,0x80,0x9e,
    0x40,0xaa,0x8d,0x34,0x08,0xa7,0x88,0x39,
    0xb3,0x34,0xe2,0xa8,0x73,0x70,0xe4,0x56,
    0xa1,0xba,0x31,0x35,0xfa,0x23,0x3a,0x7a,
    0xb0,0x95,0x7a,0xc3,0xca,0x7b,0x26,0xaf,
    0x77,0x32,0xab,0x86,0x00,0x80,0x1d,0x68,
    0x56,0x62,0xba,0x4e,0x6c,0x4b,0x83,0x6e,
    0xe5,0x1e,0xfe,0x62,0x94,0xdf,0x1b,0x18,
    0xaa,0x10,0x6e,0x75,0xc0,0x1a,0x51,0x43,
    0x99,0xdb,0x85,0x98,0x9a,0x7b,0xbb,0x97
};

static unsigned char    ca_exponent[] = {
    0x01,0x00,0x01
};

SOCLCaInfo      ca_info = {
    ca_name,
    sizeof(ca_modulo),
    ca_modulo,
    sizeof(ca_exponent),
    ca_exponent
};

SOCLCaInfo*     ca_list[] = { &ca_info };

/*****************************************************************************/

/* function */

/*---------------------------------------------------------------------------*
  Name:         SslAuthCallback

  Description:  SSL サーバ認証コールバック.

  Arguments:    con           SSL接続情報
                level         レベル

  Returns:      None.
 *---------------------------------------------------------------------------*/
int SslAuthCallback(int result, SOCSslConnection* con, int level)
{
#pragma unused(con)
#pragma unused(level)
    {
        OS_TPrintf("SSL: %d s:<%s>\n", level, con->subject);
        OS_TPrintf("SSL:     /CN=%s/\n", con->cn);
        OS_TPrintf("SSL:   i:<%s>\n", con->issuer);
    }

    if (result & SOC_CERT_OUTOFDATE)
    {
        OS_TPrintf("SSL: Certificate is out-of-date\n");
    }

    if (result & SOC_CERT_BADSERVER)
    {
        OS_TPrintf("SSL: Server name does not match\n");
        result &= ~SOC_CERT_BADSERVER;
    }

    switch (result & SOC_CERT_ERRMASK)
    {
    case SOC_CERT_NOROOTCA:
        OS_TPrintf("SSL: No root CA installed.(ignored)\n");
        result &= ~SOC_CERT_ERRMASK;    //ignore
        break;

    case SOC_CERT_BADSIGNATURE:
        OS_TPrintf("SSL: Bad signature\n");
        break;

    case SOC_CERT_UNKNOWN_SIGALGORITHM:
        OS_TPrintf("SSL: Unknown signature algorithm\n");
        break;

    case SOC_CERT_UNKNOWN_PUBKEYALGORITHM:
        OS_TPrintf("SSL: Unknown public key alrorithm\n");
        break;
    }

    return 0;   //result;
}

/*---------------------------------------------------------------------------*
  Name:         SslInit

  Description:  SSL 初期化.

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void SslInit(void)
{
    /* 乱数の追加は頻繁に行うほうが良い */
    static u32  ssl_seed[OS_LOW_ENTROPY_DATA_SIZE / sizeof(u32)];
    OS_GetLowEntropyData(ssl_seed);
    CPS_SslAddRandomSeed(ssl_seed, sizeof(ssl_seed));

    if (!g_psslCon)
    {
        MI_CpuClear8(&g_sslCon, sizeof(SOCSslConnection));
        g_sslCon.server_name = NULL;;

        SslSetDefaultConncetion(&g_sslCon);
        SslSetDefaultCA(ca_list, sizeof(ca_list) / sizeof(ca_list[0]));
        SslSetDefaultAuthCallback(SslAuthCallback);
    }
}

/*---------------------------------------------------------------------------*
  Name:         SslGetConnection

  Description:  ssl.cに登録されているSOCSslConnectionへのポインタを返す.
                InitSSL()後はデフォルトのパラメータで初期化されているので、
                そのままSOC_EnableSsl()で使用可能。

  Arguments:    None.

  Returns:      ssl.cに登録されているSOCSslConnectionへのポインタ.
                InitSSL前はNULLを返す.
 *---------------------------------------------------------------------------*/
SOCSslConnection* SslGetDefaultConnection()
{
    return g_psslCon;
}

/*---------------------------------------------------------------------------*
  Name:         SslSetConncetion

  Description:  SOCSslConnection構造体をssl.cに登録する.

  Arguments:    psslCon		登録するSOCSslConnectionへのポインタ.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void SslSetDefaultConncetion(SOCSslConnection* psslCon)
{
    if (psslCon)
    {
        g_psslCon = psslCon;
    }
    else
    {
        g_psslCon = &g_sslCon;
    }
}

/*---------------------------------------------------------------------------*
  Name:         SslSetAuthCallback

  Description:  SSL認証用のコールバックを設定する.

  Arguments:    auth_callback	コールバック関数へのポインタ.

  Returns:		None.
 *---------------------------------------------------------------------------*/
void SslSetDefaultAuthCallback(int (*auth_callback) (int, struct _CPSSslConnection*, int))
{
    g_psslCon->auth_callback = auth_callback;
}

/*---------------------------------------------------------------------------*
  Name:         SslSetDefaultCA(

  Description:  SSLの証明書を設定する.

  Arguments:    ca_list		証明書リスト
                builtins.	証明書の数

  Returns:		None.
 *---------------------------------------------------------------------------*/
void SslSetDefaultCA(SOCCaInfo ** ca_list, int builtins)
{
    g_psslCon->ca_info = ca_list;
    g_psslCon->ca_builtins = builtins;
}

/*---------------------------------------------------------------------------*
  Name:         SslCreateConnection

  Description:  SSLの証明書を設定する.

  Arguments:    servername	CNとのマッチさせる接続先のサーバ名

  Returns:		新しく確保されたSOCSslConnection構造体へのポインタ
 *---------------------------------------------------------------------------*/
SOCSslConnection* SslCreateConnection(const char* servername)
{
    SOCSslConnection*   con = OS_Alloc(sizeof(SOCSslConnection));
    MI_CpuCopy8(g_psslCon, con, sizeof(SOCSslConnection));
    if (servername)
    {
        con->server_name = OS_Alloc((u32) STD_StrLen(servername) + 1);
        (void)STD_CopyString(con->server_name, servername);
    }

    return con;
}

/*---------------------------------------------------------------------------*
  Name:         SslDestroyConnection

  Description:  SslCreateConnection構造体を破棄する.

  Arguments:    con		SOCSslConnection構造体へのポインタ

  Returns:		なし
 *---------------------------------------------------------------------------*/
void SslDestroyConnection(SOCSslConnection* con)
{
    if (con)
    {
        if (con->server_name)
        {
            OS_Free(con->server_name);
        }

        OS_Free(con);
    }
}

void SslPrintCA(const SOCSslConnection* con)
{
    int index;
    for (index = 0; index < con->ca_builtins; ++index)
    {
        OS_TPrintf("%d: %s\n", index, g_psslCon->ca_info[index]->dn);
    }
}
