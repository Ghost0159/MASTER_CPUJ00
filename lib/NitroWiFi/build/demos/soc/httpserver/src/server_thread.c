/*---------------------------------------------------------------------------*
  Project:  NitroWiFi - SOC - demos
  File:     server_thread.c

  Copyright 2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: server_thread.c,v $
  Revision 1.6  2006/07/18 02:37:09  okubata_ryoma
  NitroSDK3.2PR以前のSTD_CopyLStringと互換性を保つための変更

  Revision 1.5  2006/03/10 09:22:43  kitase_hirotake
  INDENT SOURCE

  Revision 1.4  2006/01/16 00:05:18  kitase_hirotake
  送信メッセージの変更

  Revision 1.3  2006/01/10 04:09:45  kitase_hirotake
  不必要な break 文の削除

  Revision 1.2  2006/01/06 04:30:44  kitase_hirotake
  通信の高速化

  Revision 1.1  2006/01/06 00:43:29  kitase_hirotake
  httpserver の追加

  $NoKeywords: $
 *---------------------------------------------------------------------------*/
#include <nitro.h>
#include <nitroWiFi.h>
#include <nitro/fs.h>
#include "server_thread.h"

#define SEND_SIZE       1500
#define RECV_SIZE       128
#define REQMETHOD_SIZE  8
#define FILEPATH_SIZE   64
#define HTTPVER_SIZE    32
#define MIMETYPE_SIZE   32
#define STACK_SIZE      8192

/* サーバスレッドは、ランチャスレッド (Accept を行う) よりも優先度は低い */
#define HTTPSERVER_PRIO 20

/* Content-type 用の MIME 設定表 */
typedef struct mimedef
{
    u8      number;
    char    mime[16];
} mimedef;

typedef struct numberdef
{
    char    filetype[8];
    u8      number;
} numberdef;

/* 拡張子と Content-type の対応表 */
mimedef     Mime_Data[] = { {1, "image/gif"},
    {2, "image/jpeg"},
    {3, "text/html"},
    {4, "text/plain"},
    {0, "end"},
};

numberdef   Filetype_Data[] = { {"gif", 1},
    {"jpeg", 2},
    {"jpg", 2},
    {"jpe", 2},
    {"html", 3},
    {"htm", 3},
    {"txt", 4},
    {"end", 0},
};

#define MethodCheck(request_method) (STD_StrCmp(request_method, "GET") == 0 ? 1 : 0)

extern OSMessageQueue   mesgQueue;

/* ファイルから 1 行読み込む */
static char* my_fgets(char* buf, int len, FSFile* p_file)
{
    int ret = FS_ReadFile(p_file, buf, len - 1);
    if (ret < 0)
        return NULL;
    else
    {
        int i;
        for (i = 0; i < ret; ++i)
        {
            u32 c = MI_ReadByte(buf + i);
            if (c == '\n')
            {
                ++i;
                break;
            }
        }

        MI_WriteByte(buf + i, (u8) '\0');
        (void)FS_SeekFile(p_file, i - ret, FS_SEEK_CUR);
    }

    return buf;
}

/* スレッドに必要なメモリを確保する */
static void AllocThreadStorage(OSThread ** ppThread, void ** ppStackTop)
{
    OSIntrMode  enable;

    /* Warningが出ないように空きメモリをチェック */
    if (OS_GetMaxFreeSize(OS_ARENA_MAIN, OS_CURRENT_HEAP_HANDLE) < STACK_SIZE ||
        OS_GetTotalFreeSize(OS_ARENA_MAIN, OS_CURRENT_HEAP_HANDLE) < STACK_SIZE + sizeof(OSThread))
    {
        OS_TPrintf("\n AllocThreadStorage MISS!\n\n");
        *ppThread = NULL;
        *ppStackTop = NULL;
        return;
    }

    enable = OS_DisableInterrupts();
    *ppThread = OS_Alloc(sizeof(OSThread));
    *ppStackTop = OS_Alloc(STACK_SIZE);
    (void)OS_RestoreInterrupts(enable);
}

/*---------------------------------------------------------------------------*
  Name:         CreateServerThread

  Description:  サーバスレッドを作成.

  Arguments:    None.

  Returns:      なし.
 *---------------------------------------------------------------------------*/
void CreateServerThread(void)
{
    OSThread*   pThread = NULL;
    void*       pStackTop = NULL;
    AllocThreadStorage(&pThread, &pStackTop);

    if (!pThread || !pStackTop)
    {
        return;
    }

    OS_CreateThread(pThread, ServerThread, NULL, (void*)((u8*)pStackTop + STACK_SIZE), STACK_SIZE, HTTPSERVER_PRIO);

    /* サーバスレッドを起こす */
    OS_WakeupThreadDirect(pThread);
}

/* スレッドが使用していたメモリを開放 */
static void FreeThreadStorage(void* arg)
{
#pragma unused(arg)
    OSIntrMode  enable = OS_DisableInterrupts();

    OS_Free((void*)OS_GetCurrentThread()->stackTop);
    OS_Free(OS_GetCurrentThread());

    (void)OS_RestoreInterrupts(enable);
}

/* 月の表記の変換 */
static char* ConvertMonth(u32 month)
{
    switch (month)
    {
    case 1:
        return "Jan";

    case 2:
        return "Feb";

    case 3:
        return "Mar";

    case 4:
        return "Apr";

    case 5:
        return "May";

    case 6:
        return "Jun";

    case 7:
        return "Jul";

    case 8:
        return "Aug";

    case 9:
        return "Sep";

    case 10:
        return "Oct";

    case 11:
        return "Nov";

    case 12:
        return "Dec";

    default:
        return "Jan";
    }
}

/* 曜日の表記の変更 */
static char* ConvertWeek(RTCWeek week)
{
    switch (week)
    {
    case RTC_WEEK_SUNDAY:
        return "Sun";

    case RTC_WEEK_MONDAY:
        return "Mon";

    case RTC_WEEK_TUESDAY:
        return "Tue";

    case RTC_WEEK_WEDNESDAY:
        return "Wed";

    case RTC_WEEK_THURSDAY:
        return "Thu";

    case RTC_WEEK_FRIDAY:
        return "Fri";

    case RTC_WEEK_SATURDAY:
        return "Sat";

    default:
        return "Sun";
    }
}

/* 東京時間をグリニッジ標準時間に変換する */
static void ConvertGmt(RTCDate* date, RTCTime* time)
{
    s64 total_second;

    /* グリニッジ標準時(GMT)は東京より9時間早い */
    total_second = RTC_ConvertDateTimeToSecond(date, time);
    total_second -= 32400;
    RTC_ConvertSecondToDateTime(date, time, total_second);
}

/* クライアントにデータを送信する */
static int SendData(int socket, char* buffer)
{
    int result;

    do
    {
        result = SOC_Write(socket, buffer, STD_StrLen(buffer));

        /* 再送は 17msec 待ってから */
        if (result == SOC_EAGAIN)
        {
            OS_Sleep(17);
        }
    }
    while (result == SOC_EAGAIN);

    return result;
}

/* ファイルのタイプ( text/html 等)を得る */
static BOOL FiletypeCheck(char* filepath, char* mimetype)
{
    char    filetype[8];
    u8      filetype_num;
    int     i;

    i = STD_StrLen(filepath);
    for (; filepath[i] != '.'; i--)
    {
        if (i <= 0)
        {
            /* 拡張子のないファイルである */
            mimetype = NULL;
            return FALSE;
        }
    }

    // NitroSDK3.2PR以前のSTD_CopyLStringと互換性を保つため
    MI_CpuClear8(filetype, sizeof(filetype));

    i++;
    (void)STD_StrLCpy(filetype, &filepath[i], 7);

    for (i = 0; STD_StrCmp(filetype, Filetype_Data[i].filetype); i++)
    {
        if (Filetype_Data[i].number == 0)
        {
            /* 一致する拡張子がないファイルである */
            mimetype = NULL;
            return FALSE;
        }
    }

    filetype_num = Filetype_Data[i].number;

    for (i = 0; Mime_Data[i].number != filetype_num; i++)
    {
        if (Mime_Data[i].number == 0)
        {
            /* 一致する MIME がないファイルである */
            mimetype = NULL;
            return FALSE;
        }
    }
    (void)STD_StrCpy(mimetype, Mime_Data[i].mime);
    return TRUE;
}

/* リクエストメソッドが GET であるか判定する */
static BOOL CheckGetMethod(char* read_buffer, char* request_method, char* filepath, char* http_version)
{
    BOOL    check = TRUE;
    int     i, j;

    for (i = 0; read_buffer[i] != ' '; i++)
    {
        if (i > REQMETHOD_SIZE)
        {
            check = FALSE;
            break;
        }

        request_method[i] = read_buffer[i];
        if (read_buffer[i] == '\0')
        {
            check = FALSE;
            break;
        }
    }

    if (check == TRUE)
    {
        request_method[i] = '\0';
        i++;

        /* 現状では、GET メソッドのみを許す */
        if (!MethodCheck(request_method))
        {
            check = FALSE;
        }
    }

    if (check == TRUE)
    {
        /* 取得したメソッド別の処理へ */

        /* 現状では、GET メソッドのみ対応する */
        for (j = 0; read_buffer[i] != ' '; i++, j++)
        {
            if (j > FILEPATH_SIZE)
            {
                check = FALSE;
                break;
            }

            filepath[j] = read_buffer[i];
            if (read_buffer[i] == '\0')
            {
                check = FALSE;
                break;
            }
        }

        if (check == TRUE)
        {
            filepath[j] = '\0';
            i++;

            /* ルートディレクトリの指定のみならば、index.htmlにする */
            if ((filepath[0] == '/') && (filepath[1] == '\0'))
            {
                (void)STD_StrCpy(filepath, "/index.html");
            }

            /* http_version には HTTP リクエストの HTTP バージョンが入っている */
            for (j = 0; read_buffer[i] != '\n'; i++, j++)
            {
                http_version[j] = read_buffer[i];
                if (read_buffer[i] == '\0')
                {
                    check = FALSE;
                    break;
                }
            }
        }
    }

    /* これ移行ヘッダ情報があるが、無視する */
    return check;
}

/* GET に対するレスポンスを送信する */
static void SendResponse(int client_socket, char* filepath)
{
    FSFile  file;
    BOOL    open_is_ok;
    int     result;

    char    send_buffer[SEND_SIZE];

    FS_InitFile(&file);
    open_is_ok = FS_OpenFile(&file, filepath);
    OS_TPrintf("FS_OpenFile(\"%s\") ... %s!\n", filepath, open_is_ok ? "OK" : "ERROR");
    if (!open_is_ok)
    {
        if (FS_OpenFile(&file, STD_StrCpy(filepath, "/NotFound.html")) == FALSE)
        {
            return;
        }
    }
    {
        RTCDate date;
        RTCTime time;
        int     length;
        char    mimetype[MIMETYPE_SIZE];

        /* HTTP レスポンスを送信 */

        /* ステータス行          *
         * HTTP バージョンは 1.0 */
        if (open_is_ok)
        {
            result = SendData(client_socket, "HTTP/1.0 200 OK\r\n");
        }
        else
        {
            result = SendData(client_socket, "HTTP/1.0 404 Not Found\r\n");
        }

        /* 付加情報 */
        result = SendData(client_socket, "Server: NINTENDO DS\r\n");

        result = SendData(client_socket, "Connection: close\r\n");

        /* 日時情報を得る */
        if (RTC_GetDateTime(&date, &time) != 0)
        {
            /* 日時取得の失敗 */
            date.year = 2005;
            date.month = 12;
            date.day = 24;
            date.week = RTC_WEEK_SATURDAY;
            time.hour = 19;
            time.minute = 50;
            time.second = 0;
        }

        ConvertGmt(&date, &time);
        length = OS_SNPrintf(send_buffer, SEND_SIZE, "Date: %s, %d %s %d %2d:%2d:%2d GMT\r\n", ConvertWeek(date.week),
                             date.day, ConvertMonth(date.month), 2000 + date.year, time.hour, time.minute, time.second);
        result = SOC_Write(client_socket, send_buffer, length);

        /* MIME TYPE を指定する */

        /* ファイル名の拡張子から推測する */
        if (FiletypeCheck(filepath, mimetype))
        {
            length = OS_SNPrintf(send_buffer, SEND_SIZE, "Content-type: %s\r\n", mimetype);
            result = SOC_Write(client_socket, send_buffer, length);
        }
        else
        {
            /* 拡張子のないファイルの送信要求だったなら、text/plain として送る */
            result = SendData(client_socket, "Content-type: text/plain\r\n");
        }

        /* ファイルサイズ(byte) */
        length = OS_SNPrintf(send_buffer, SEND_SIZE, "Content-Length: %d\r\n", FS_GetLength(&file));
        result = SOC_Write(client_socket, send_buffer, length);

        result = SendData(client_socket, "\r\n");

        /* エンティティボディの送信 */
        while (TRUE)
        {
            int ret = FS_ReadFile(&file, send_buffer, SEND_SIZE);
            if (ret > 0)
            {
                do
                {
                    result = SOC_Write(client_socket, send_buffer, ret);

                    /* 再送は 17msec 待ってから */
                    if (result == SOC_EAGAIN)
                    {
                        OS_Sleep(17);
                    }
                }
                while (result == SOC_EAGAIN);
            }
            else
            {
                break;
            }
        }
        (void)FS_CloseFile(&file);
    }
}

/*---------------------------------------------------------------------------*
  Name:         ServerThread

  Description:  http クライアントとの通信.

  Arguments:    arg.

  Returns:      なし.
 *---------------------------------------------------------------------------*/
void ServerThread(void* arg)
{
#pragma unused(arg)

    int     result;

    char    request_method[REQMETHOD_SIZE];
    char    filepath[FILEPATH_SIZE];
    char    http_version[HTTPVER_SIZE];

    int     client_socket;

    char    read_buffer[RECV_SIZE];

    /* スレッドのデストラクタの設定 */
    OS_SetThreadDestructor(OS_GetCurrentThread(), FreeThreadStorage);

    while (1)
    {
        OSMessage   message;

        /* クライアントの接続が来るのをキューで待つ */
        (void)OS_ReceiveMessage(&mesgQueue, &message, OS_MESSAGE_BLOCK);

        client_socket = *((int*)message);

        while (1)
        {
            /* GET コマンドを受信する */
            result = SOC_Recv(client_socket, read_buffer, RECV_SIZE - 1, SOC_MSG_DONTWAIT);
            if (result != SOC_EWOULDBLOCK)
            {
                if (result < 0)
                {
                    /* Recv のエラーだったので、このスレッドを終了させる */
                    OS_TPrintf("Socket Error Code = %d\n", result);
                    result = SOC_Close(client_socket);
                    do
                    {
                        result = SOC_Close(client_socket);
                        if ((result != SOC_EINPROGRESS) && (result < 0))
                        {
                            /* ソケットが正常に閉じられなかった */
                            OS_TPrintf("Socket Close Error!\n");
                            break;
                        }
                    }
                    while (result == SOC_EINPROGRESS);
                    break;
                }

                if (result > 0)
                {
                    read_buffer[result] = '\0';
                }

                /* 送信されたリクエストメソッドを取得 */

                /* GET メソッドが正当であったなら、FS からファイル情報を送信 */
                if (CheckGetMethod(read_buffer, request_method, filepath, http_version) == TRUE)
                {
                    SendResponse(client_socket, filepath);

                    /* 1 回のやりとりで切断する */
                    do
                    {
                        result = SOC_Close(client_socket);
                        if ((result != SOC_EINPROGRESS) && (result < 0))
                        {
                            /* ソケットが正常に閉じられなかった */
                            OS_TPrintf("Socket Close Error!\n");
                            break;
                        }
                    }
                    while (result == SOC_EINPROGRESS);
                    break;
                }
            }
            else
            {
                OS_Sleep(17);
            }
        }
    }
}
