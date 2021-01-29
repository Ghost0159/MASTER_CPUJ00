/*---------------------------------------------------------------------------*
  Project:  NitroWiFi - SOC - demos - httpclient
  File:     http.c

  Copyright 2005,2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: http.c,v $
  Revision 1.2  2006/03/10 09:22:43  kitase_hirotake
  INDENT SOURCE

  Revision 1.1  2005/12/22 04:34:06  adachi_hiroaki
  HTTPクライアントのデモをSOCで置き換え

  $NoKeywords$
 *---------------------------------------------------------------------------*/
#include <nitro.h>
#include <nitroWiFi.h>
#include "http.h"
#include "ssl.h"

#define REQUEST_BUFFER_SIZE 4096

static int  Writen(int sock, const char* buf, int len);
static int  Readn(int sock, char* buf, int len);

/*---------------------------------------------------------------------------*
  Name:         HttpConnect

  Description:  URLを解析してHttp/HttpSサーバに接続する

  Arguments:    url         - 接続先のURL
                sslCon      - SOCSslConnection構造体へのポインタのポインタ

  Returns:      >= 0 なら成功.
                忘れずにCloseすること.
 *---------------------------------------------------------------------------*/
int HttpConnect(const char* url, SOCSslConnection ** sslCon)
{
    HttpParsedURL   parsedUrl;
    SOCSockAddrIn   addrin;
    int result;
    int socket = -1;

    *sslCon = NULL;

    if (!HttpParseURL(&parsedUrl, url))
    {
        return SOC_EINVAL;
    }

    socket = SOC_Socket(SOC_PF_INET, SOC_SOCK_STREAM, 0);
    if (socket < 0)
    {
        return socket;
    }

    //名前を解決してSOCSockAddrInの値を設定
    {
        SOCHostEnt*     hostent = NULL;
        {
            //SOC_GetHostByName() is not thread safe
            static OSMutex  mutex;
            {
                static BOOL bFirst = TRUE;
                if (bFirst)
                {
                    OS_InitMutex(&mutex);
                    bFirst = FALSE;
                }

                OS_LockMutex(&mutex);
                hostent = SOC_GetHostByName(parsedUrl.host);
                OS_UnlockMutex(&mutex);
            }
        }

        if (hostent == NULL)
        {
            result = SOC_EINVAL;
            goto cleanup;
        }

        MI_CpuCopy8(hostent->addrList[0], &addrin.addr, hostent->length);
        addrin.len = sizeof(addrin);
        addrin.family = SOC_AF_INET;
        addrin.port = SOC_HtoNs(parsedUrl.port);
    }

    //HttpSならSSLを有効に
    if (sslCon && STD_CompareString(parsedUrl.scheme, "https") == 0)
    {
        *sslCon = SslCreateConnection(parsedUrl.host);
        if (!*sslCon || (result = SOC_EnableSsl(socket, *sslCon)) < 0)
        {
            goto cleanup;
        }
    }

    //接続開始
    result = SOC_Connect(socket, &addrin);
    if (result < 0)
    {
        goto cleanup;
    }

    return socket;

cleanup:
    (void)HttpClose(socket, *sslCon);
    return result;
}

/*---------------------------------------------------------------------------*
  Name        : HttpClose
  Description : 
  Arguments   : socket      - HttpConnect()の返り値
                sslCon      - 破棄するSOCSslConnectionへのポインタ
  Returns     : 
 *---------------------------------------------------------------------------*/
int HttpClose(int socket, SOCSslConnection* sslCon)
{
    if (socket >= 0)
    {
        (void)SOC_EnableSsl(socket, NULL);
        SslDestroyConnection(sslCon);
        return SOC_Close(socket);
    }

    return 0;
}

/*---------------------------------------------------------------------------*
  Name        : HttpGet
  Description : HTTPリクエストを発行しページを取得する
  Arguments   : socket      - HttpConnect()の返り値
                url         - 対象のURL
                buf         - データを格納するバッファ
                buflen      - データを格納するバッファ長
  Returns     : 
 *---------------------------------------------------------------------------*/
int HttpGet(int socket, const char*  url, const char*  path_info, const HttpParam*  params, int param_num, char*  buf,
            int buflen)
{
    HttpParsedURL*  parsed_url = NULL;
    char*           request_buffer = NULL;
    char*           request_path = NULL;
    int     result = 0;
    int     len;
    BOOL    bAutoConnect = FALSE;

    SDK_ASSERT(url != NULL);
    SDK_ASSERT(params != NULL || param_num == 0);

    // parse url
    parsed_url = OS_Alloc(sizeof(HttpParsedURL));
    if (!parsed_url)
    {
        result = SOC_ENOMEM;
        goto error;
    }

    if (!HttpParseURL(parsed_url, url))
    {
        result = SOC_EINVAL;
        goto error;
    }

    // prepare path
    request_path = OS_Alloc(HTTP_REQUEST_PATH_MAX);
    if (!request_path)
    {
        result = SOC_ENOMEM;
        goto error;
    }

    if (param_num == 0)
    {
        if (path_info == NULL)
        {
            MI_CpuCopy8(parsed_url->path, request_path, HTTP_REQUEST_PATH_MAX - 1);
            request_path[HTTP_REQUEST_PATH_MAX - 1] = 0;
        }
        else
        {
            len = OS_SNPrintf(request_path, HTTP_REQUEST_PATH_MAX, "%s/%s", parsed_url->path, path_info);
        }
    }
    else
    {
        char*   query_string;
        int     index;
        int     i_param;
        query_string = OS_Alloc(HTTP_REQUEST_PATH_MAX);
        if (!query_string)
        {
            result = SOC_ENOMEM;
            goto error;
        }

        index = 0;

        query_string[0] = '\0';
        for (i_param = 0; i_param < param_num; i_param++)
        {
            if (index >= HTTP_REQUEST_PATH_MAX - 1)
            {
                OS_Free(query_string);
                result = SOC_EINVAL;
                goto error;
            }

            if (i_param != 0)
            {
                query_string[index] = '&';
                index++;
            }

            len = OS_SNPrintf(&query_string[index], (u32) (HTTP_REQUEST_PATH_MAX - index), "%s=%s",
                              params[i_param].name, params[i_param].value);
            index += len;
        }

        if (path_info == NULL)
        {
            len = OS_SNPrintf(request_path, HTTP_REQUEST_PATH_MAX, "%s?%s", parsed_url->path, query_string);
        }
        else
        {
            len = OS_SNPrintf(request_path, HTTP_REQUEST_PATH_MAX, "%s/%s?%s", parsed_url->path, path_info, query_string);
        }

        OS_Free(query_string);
    }

    OS_TPrintf("downloading %s\n", parsed_url->path);

    request_buffer = OS_Alloc(REQUEST_BUFFER_SIZE);
    if (!request_buffer)
    {
        result = SOC_ENOMEM;
        goto error;
    }

    // send http request
    len = OS_SNPrintf(request_buffer, REQUEST_BUFFER_SIZE, "GET %s HTTP/1.0\r\nHost: %s\r\nConnection: close\r\n\r\n",
                      request_path, parsed_url->host);

    result = Writen(socket, request_buffer, len);
    if (result < 0)
    {
        goto error;
    }

    // receive http response
    result = Readn(socket, buf, buflen - 1);
    if (result < 0)
    {
        goto error;
    }

    buf[result] = '\0';

error:
    if (request_buffer)
    {
        OS_Free(request_buffer);

        //request_buffer = NULL;
    }

    if (request_path)
    {
        OS_Free(request_path);

        //request_path = NULL;
    }

    if (parsed_url)
    {
        OS_Free(parsed_url);

        //parsed_url = NULL;
    }

    return result;
}

/*-----------------------------------------------------------------------*
                    ローカル関数定義
 *-----------------------------------------------------------------------*/
static const char* skip_word(const char* str, const char* word)
{
    while (*word != '\0' && *str == *word)
    {
        str++;
        word++;
    }

    if (*word == '\0')
    {
        return str;
    }
    else
    {
        return NULL;
    }
}

static const char* skip_until_chars(const char* str, const char* chars)
{
    while (*str != '\0')
    {
        const char*     c;
        for (c = chars; *c != 0; c++)
        {
            if (*str == *c)
            {
                return str;
            }
        }

        str++;
    }

    return str;
}

static BOOL conv_str2int(int* num, const char* str, int len)
{
    int n = 0;
    while (len > 0)
    {
        if (*str < '0' || '9' < *str)
        {
            return FALSE;
        }

        n *= 10;
        n += *str - '0';
        str++;
        len--;
    }

    *num = n;
    return TRUE;
}

BOOL HttpParseURL(HttpParsedURL* parsed_url, const char* url)
{
    const char*     curr = url, *p = NULL;
    int len;

    SDK_ASSERT(parsed_url != NULL);
    SDK_ASSERT(url != NULL);

    // defaults
    parsed_url->host[0] = 0;
    (void)strcpy(parsed_url->path, "/");
    parsed_url->port = 80;

    // protocol
    if (curr == NULL)
    {
        return FALSE;
    }

    p = skip_until_chars(curr, ":");
    p = skip_word(p, "://");
    if (p == NULL)
    {
        return FALSE;
    }

    p -= 3; //scheme tail
    len = p - curr;
    if (len == 0 || len >= SCHEME_MAX)
    {
        return FALSE;
    }

    MI_CpuCopy8(curr, parsed_url->scheme, (u32) len);

    if (STD_CompareString(parsed_url->scheme, "http") == 0)
    {
        parsed_url->port = 80;
    }
    else if (STD_CompareString(parsed_url->scheme, "https") == 0)
    {
        parsed_url->port = 443;
    }

    // host
    curr = skip_word(p, "://");
    p = skip_until_chars(curr, ":/");
    len = p - curr;
    if (len == 0 || len >= HOST_NAME_MAX)
    {
        return FALSE;
    }

    MI_CpuCopy8(curr, parsed_url->host, (u32) len);
    parsed_url->host[len] = 0;
    curr = p;

    // port
    if (*curr == ':')
    {
        curr++;
        p = skip_until_chars(curr, "/");
        len = p - curr;
        if (len > 0)
        {
            int port;
            if (conv_str2int(&port, curr, len))
            {
                parsed_url->port = port;
            }
            else
            {
                return FALSE;
            }
        }

        curr = p;
    }

    // path
    if (*curr == '/')
    {
        len = (int)strlen(curr);
        if (len >= HTTP_REQUEST_PATH_MAX)
        {
            return FALSE;
        }

        MI_CpuCopy8(curr, parsed_url->path, (u32) len);
        parsed_url->path[len] = 0;
    }

    return TRUE;
}

static int Writen(int sock, const char* buf, int len)
{
    int i;
    int result;

    for (i = 0; i < len;)
    {
        result = SOC_Write(sock, buf + i, len - i);
        if (result < 0)
        {
            goto exit;
        }

        i += result;
    }

    result = i;

exit:
    if (result < 0)
    {
        OS_TPrintf("SOC_Write: %d\n", result);
    }

    return result;
}

static int Readn(int sock, char* buf, int len)
{
    int i;
    int result;

    for (i = 0; i < len;)
    {
        result = SOC_Read(sock, buf + i, len - i);
        if (result < 0)
        {
            goto exit;
        }

        if (result == 0)
        {
            break;
        }

        i += result;
    }

    result = i;

exit:
    if (result < 0)
    {
        OS_TPrintf("SOC_Read: %d\n", result);
    }

    return result;
}
