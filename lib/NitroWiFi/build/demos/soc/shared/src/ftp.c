/*---------------------------------------------------------------------------*
  Project:  NitroWiFi - demo - soc - shared
  File:     ftp.c

  Copyright 2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: ftp.c,v $
  Revision 1.5  2006/07/18 02:37:09  okubata_ryoma
  NitroSDK3.2PR以前のSTD_CopyLStringと互換性を保つための変更

  Revision 1.4  2006/07/18 00:42:08  okubata_ryoma
  small fix

  Revision 1.3  2006/07/18 00:06:14  okubata_ryoma
  STD_CopyLStringの仕様変更による変更

  Revision 1.2  2006/03/10 09:22:43  kitase_hirotake
  INDENT SOURCE

  Revision 1.1  2006/01/12 02:16:41  okubata_ryoma
  ftp.ｃの追加


  $NoKeywords: $
 *---------------------------------------------------------------------------*/
#include <nitro.h>
#include "ftp.h"

static int      FTPi_CommandSocket = -1;
static int      FTPi_DataSocket = -1;

static char     send_buf[FTPi_BUFFERSIZE_UP];

int         FTPi_Readline(char* buffer, size_t size);
int         FTPi_Printf(const char* fmt, ...);
int         FTPi_GetStatus(const char* line);
int         ConnectToFtpServer(const char* http_addr, u16 http_port);
int         FTP_TransferType(const char type);
BOOL        FTPi_ReadMoreData(char* line);
BOOL        FTPi_IsMoreDataAvailable(const char* line);
BOOL        FTPi_IsSuccess(const char* line);

static char*    Upload_fgets(char* buf, int len, FSFile* p_file);

/*****************************************************************************/

/* function */

/*---------------------------------------------------------------------------*
  Name:         FTP_Open

  Description:  FTPサーバに接続する.

  Arguments:    http_addr     : 接続先アドレス
                http_port     : 接続先ポート

  Returns:      >= 0なら成功.
 *---------------------------------------------------------------------------*/
int FTP_Open(const char* http_addr, u16 http_port)
{
    // サーバにコマンド用のコネクションを張る
    FTPi_CommandSocket = ConnectToFtpServer(http_addr, http_port);

    return FTPi_CommandSocket;
}

/*---------------------------------------------------------------------------*
  Name:         FTP_Login

  Description:  USERとPASSコマンドを発行しFTPサーバにログインする.

  Arguments:    user_name     : 接続先アカウント
                password      : 接続先パスワード
                

  Returns:      >= 0なら成功.
 *---------------------------------------------------------------------------*/
int FTP_Login(const char* user_name, const char* password)
{
    char    recv_buf[FTPi_BUFFERSIZE_COMMAND];

    // Connectに対するメッセージを受信
    FTPi_RETURN_ERROR(FTPi_Readline(recv_buf, FTPi_BUFFERSIZE_COMMAND));

    // 応答コマンドを検査
    if (!FTPi_IsSuccess(recv_buf))
    {
        return -1;
    }

    // 複数行の応答の場合、最後まで読み続ける
    if (!FTPi_ReadMoreData(recv_buf))
    {
        return -1;
    }

    // UserName
    FTPi_RETURN_ERROR(FTPi_Printf("USER %s\r\n", user_name));
    FTPi_RETURN_ERROR(FTPi_Readline(recv_buf, FTPi_BUFFERSIZE_COMMAND));

    // 応答コマンドを検査
    if (!FTPi_IsSuccess(recv_buf))
    {
        return -1;
    }

    // 複数行の応答の場合、最後まで読み続ける
    if (!FTPi_ReadMoreData(recv_buf))
    {
        return -1;
    }

    // Password
    FTPi_RETURN_ERROR(FTPi_Printf("PASS %s\r\n", password));
    FTPi_RETURN_ERROR(FTPi_Readline(recv_buf, FTPi_BUFFERSIZE_COMMAND));

    // 応答コマンドを検査
    if (!FTPi_IsSuccess(recv_buf))
    {
        return -1;
    }

    // 複数行の応答の場合、最後まで読み続ける
    if (!FTPi_ReadMoreData(recv_buf))
    {
        return -1;
    }

    return 0;
}

/*---------------------------------------------------------------------------*
  Name:         FTP_Passive

  Description:  Passiveモードでデータコネクションを確立する.

  Arguments:    filetype ファイル転送のモード TRUE  : BINARYモード
                                              FALSE : ASCIIモード

  Returns:      >= 0なら成功.
 *---------------------------------------------------------------------------*/
int FTP_Passive(BOOL filetype)
{
    char    recv_buf[FTPi_BUFFERSIZE_COMMAND];

    // FileType
    if (filetype)
    {
        FTPi_RETURN_ERROR(FTP_TransferType('I'));
    }
    else
    {
        FTPi_RETURN_ERROR(FTP_TransferType('A'));
    }

    // Passiveモード
    FTPi_RETURN_ERROR(FTPi_Printf("PASV\r\n"));
    FTPi_RETURN_ERROR(FTPi_Readline(recv_buf, FTPi_BUFFERSIZE_COMMAND));

    // 応答コマンドを検査
    if (!FTPi_IsSuccess(recv_buf))
    {
        return -1;
    }

    // 複数行の応答の場合、最後まで読み続ける
    if (!FTPi_ReadMoreData(recv_buf))
    {
        return -1;
    }

    // PASVコマンドの応答メッセージからサーバのアドレス、ポートを抜き出す
    {
        static char http_addr[16];
        static u16  http_port;
        char*       tmp_ptr;

        tmp_ptr = strchr(recv_buf, '(');
        if (tmp_ptr == NULL)
        {
            OS_TPrintf("recv_buf = %s\n", recv_buf);
            return -1;
        }

        tmp_ptr++;
        (void)strncpy(http_addr, tmp_ptr, 1);
        http_addr[1] = '\0';
        {
            int count = 0;  // ','の出現回数
            while (1)
            {
                tmp_ptr++;
                if (tmp_ptr[0] == ',')
                {
                    count++;
                    if (count == 4)
                    {
                        break;
                    }
                    (void)strncat(http_addr, ".", 1);
                }
                else
                {
                    (void)strncat(http_addr, &tmp_ptr[0], 1);
                }
            }

            tmp_ptr++;
            http_port = (u16) atoi(tmp_ptr);
            while (tmp_ptr[0] != ',')
            {
                tmp_ptr++;
            }

            tmp_ptr++;
            http_port = (u16) (http_port * 256 + atoi(tmp_ptr));
        }

        // サーバにデータ用のコネクションを張る
        FTPi_DataSocket = ConnectToFtpServer(http_addr, http_port);
    }

    return FTPi_DataSocket;
}

/*---------------------------------------------------------------------------*
  Name:         FTP_OnlyStor

  Description:  アップロードするファイルサイズを取得して、STOR コマンドを送る

  Arguments:    filename    : アップロードするファイル名

  Returns:      アップロードするファイルのサイズ、>= 0なら成功.
 *---------------------------------------------------------------------------*/
int FTP_OnlyStor(const char* filename)
{
    char    recv_buf[FTPi_BUFFERSIZE_COMMAND];
    int     buf_len;    // ファイル中の文字列の長さ
    char*   send_image;
    u32     open_filesize = 0;
    char*   return_point;

    FSFile  file;
    BOOL    open_is_ok;
    FS_InitFile(&file);

    send_image = send_buf;

    open_is_ok = FS_OpenFile(&file, filename);
    OS_TPrintf("FS_OpenFile(\"%s\") ... %s!\n", filename, open_is_ok ? "OK" : "ERROR");
    open_filesize = FS_GetLength(&file);
    if (open_is_ok)
    {
        char    buf[FILEDATA256];
        MI_CpuClear8(buf, sizeof(buf));
        MI_CpuClear8(send_buf, sizeof(send_buf));
        OS_TPrintf("\n--------------------------------\n");
        while (Upload_fgets(buf, sizeof(buf), &file) && (*buf != '\0'))
        {
            MI_CpuCopy8(buf, send_buf, sizeof(buf));
            buf_len = STD_StrLen(buf);
            send_image += buf_len;
            return_point = buf;
            while ((return_point = STD_StrStr(buf, "\r")) != NULL)
            {
                *return_point = ' ';
            }

            OS_TPrintf("  %s", buf);
        }

        OS_TPrintf("\n--------------------------------\n");
        send_image -= buf_len;
    }

    // RETR コマンド送信
    FTPi_RETURN_ERROR(FTPi_Printf("STOR %s\r\n", filename));
    FTPi_RETURN_ERROR(FTPi_Readline(recv_buf, FTPi_BUFFERSIZE_COMMAND));

    // 応答コマンドを検査
    if (!FTPi_IsSuccess(recv_buf))
    {
        return -1;
    }

    // 複数行の応答の場合、最後まで読み続ける
    if (!FTPi_ReadMoreData(recv_buf))
    {
        return -1;
    }

    return (int)open_filesize;
}

/*---------------------------------------------------------------------------*
  Name:         FTP_OnlySendData

  Description:  SOC_Send を非同期で一度行う

  Arguments:    size    : 送信するサイズ.

  Returns:      今回の send でアップロードしたサイズ.
 *---------------------------------------------------------------------------*/
int FTP_OnlySendData(int size)
{
    int result;

    size = MATH_IMin(size, FTPi_BUFFERSIZE_UP);

    result = SOC_Send(FTPi_DataSocket, send_buf, size, SOC_MSG_DONTWAIT);

    // SOC_EAGAINとSOC_ENOBUFSは負の値だがエラーでないので省く
    if ((result != SOC_EAGAIN) && (result != SOC_ENOBUFS))
    {
        FTPi_RETURN_ERROR(result);
        return (int)result;
    }

    return 0;
}

/*---------------------------------------------------------------------------*
  Name:         FTP_OnlyRetr

  Description:  RETR コマンドのみを送る

  Arguments:    filename    : ダウンロードするファイル名

  Returns:      >= 0なら成功.
 *---------------------------------------------------------------------------*/
int FTP_OnlyRetr(const char* filename)
{
    char    recv_buf[FTPi_BUFFERSIZE_COMMAND];

    // RETR コマンド送信
    FTPi_RETURN_ERROR(FTPi_Printf("RETR %s\r\n", filename));
    FTPi_RETURN_ERROR(FTPi_Readline(recv_buf, FTPi_BUFFERSIZE_COMMAND));

    // 応答コマンドを検査
    if (!FTPi_IsSuccess(recv_buf))
    {
        return -1;
    }

    // 複数行の応答の場合、最後まで読み続ける
    if (!FTPi_ReadMoreData(recv_buf))
    {
        return -1;
    }

    return 0;
}

/*---------------------------------------------------------------------------*
  Name:         FTP_OnlyRecvData

  Description:  SOC_Recv を非同期で一度行う

  Arguments:    filesize             : データ転送サイズ
                recv_filename        : NLSTコマンドのとき、受信したファイル名を入れる

  Returns:      今回の recv でダウンロードしたサイズ.
 *---------------------------------------------------------------------------*/
int FTP_OnlyRecvData(int filesize, char recv_filename[FILEDATA256])
{
    char    recv_buf[FILEDATA256];
    char    print_buf[FILEDATA256];
    char*   return_point;
    int     result;
    char*   precv_buf;
    int     filename_len;

    // NitroSDK3.2PR以前のSTD_CopyLStringと互換性を保つため
    MI_CpuClear8(print_buf, sizeof(print_buf));

    MI_CpuClear8(recv_buf, FILEDATA256);    // バッファを初期化
    result = SOC_Recv(FTPi_DataSocket, recv_buf, filesize, SOC_MSG_DONTWAIT);

    if (recv_buf[0] != NULL)
    {
        if (recv_filename)
        {
            MI_CpuCopy8(recv_buf, recv_filename, FILEDATA256);
        }
        else
        {
            precv_buf = recv_buf;
            while (*precv_buf)
            {
                return_point = STD_StrStr(precv_buf, "\n");
                *(return_point - 1) = ' ';
                filename_len = (int)(return_point - precv_buf);
                (void)STD_StrLCpy(print_buf, precv_buf, filename_len + 1);

                OS_TPrintf("  %s\n", print_buf);
                precv_buf += filename_len + 1;
            }
        }
    }

    // SOC_EAGAINとSOC_ENOBUFSは負の値だがエラーでないので省く
    if ((result != SOC_EAGAIN) && (result != SOC_ENOBUFS))
    {
        FTPi_RETURN_ERROR(result);
        return (int)result;
    }

    return result;
}

/*---------------------------------------------------------------------------*
  Name:         FTP_CloseDataSocket

  Description:  FTPi_DataSocket を閉じてコネクションを切断する

  Arguments:    none.

  Returns:      >=0 なら成功.
 *---------------------------------------------------------------------------*/
int FTP_CloseDataSocket(void)
{
    char    recv_buf[FTPi_BUFFERSIZE_COMMAND];
    int     result;

    // データ用ソケットを閉じて、コネクションを切断する
    result = SOC_Close(FTPi_DataSocket);

    FTPi_DataSocket = -1;
    FTPi_RETURN_ERROR(FTPi_Readline(recv_buf, FTPi_BUFFERSIZE_COMMAND));

    // 応答コマンドを検査
    if (!FTPi_IsSuccess(recv_buf))
    {
        return -1;
    }

    // 複数行の応答の場合、最後まで読み続ける
    if (!FTPi_ReadMoreData(recv_buf))
    {
        return -1;
    }

    return 0;
}

/*---------------------------------------------------------------------------*
  Name:         FTP_OnlyAbor

  Description:  ABOR コマンドのみを送る(応答メッセージの受信すら行わない)

  Arguments:    none.

  Returns:      >= 0なら成功.
 *---------------------------------------------------------------------------*/
int FTP_OnlyAbor()
{
    // RETR コマンド送信
    FTPi_RETURN_ERROR(FTPi_Printf("ABOR\r\n"));

    return 0;
}

/*---------------------------------------------------------------------------*
  Name:         FTP_OnlySendCommand

  Description:  FTPi_Printfを行う.

  Arguments:    fmt         : 書き込む文字列


  Returns:      >= 0なら成功.
 *---------------------------------------------------------------------------*/
int FTP_OnlySendCommand(const char* fmt, ...)
{
    // RETR コマンド送信
    FTPi_RETURN_ERROR(FTPi_Printf(fmt));

    return 0;
}

/*---------------------------------------------------------------------------*
  Name:         FTP_OnlyRecvCommand

  Description:  FTPi_Readlineを行う

  Arguments:    none.

  Returns:      >= 0なら成功.
 *---------------------------------------------------------------------------*/
int FTP_OnlyRecvCommand(void)
{
    char    recv_buf[FTPi_BUFFERSIZE_COMMAND];

    FTPi_RETURN_ERROR(FTPi_Readline(recv_buf, FTPi_BUFFERSIZE_COMMAND));

    // 応答コマンドを検査
    if (!FTPi_IsSuccess(recv_buf))
    {
        return -1;
    }

    // 複数行の応答の場合、最後まで読み続ける
    if (!FTPi_ReadMoreData(recv_buf))
    {
        return -1;
    }

    return 0;
}

/*---------------------------------------------------------------------------*
  Name:         FTP_FileSize

  Description:  指定されたファイル名のサイズを得る

  Arguments:    filename    : 調べるファイル名

  Returns:      ファイルのサイズ.
 *---------------------------------------------------------------------------*/
int FTP_FileSize(const char* filename)
{
    char    recv_buf[FTPi_BUFFERSIZE_COMMAND];
    char*   tmp_ptr;

    // Downloadするファイルのサイズ確認
    FTPi_RETURN_ERROR(FTPi_Printf("SIZE %s\r\n", filename));
    FTPi_RETURN_ERROR(FTPi_Readline(recv_buf, FTPi_BUFFERSIZE_COMMAND));

    // 応答コマンドを検査
    if (!FTPi_IsSuccess(recv_buf))
    {
        return -1;
    }

    // 複数行の応答の場合、最後まで読み続ける
    if (!FTPi_ReadMoreData(recv_buf))
    {
        return -1;
    }

    // SIZEコマンドの応答メッセージからDownLoadするファイルのサイズを取得
    tmp_ptr = strchr(recv_buf, ' ');
    if (tmp_ptr == NULL)
    {
        return -1;
    }

    tmp_ptr++;

    return atoi(tmp_ptr);
}

/*---------------------------------------------------------------------------*
  Name:         FTP_Quit

  Description:  FTP接続を終了する.

  Arguments:    なし

  Returns:      >= 0なら成功.
 *---------------------------------------------------------------------------*/
int FTP_Quit(void)
{
    char    recv_buf[FTPi_BUFFERSIZE_COMMAND];
    int     result;

    FTPi_RETURN_ERROR(FTPi_Printf("QUIT\r\n"));
    FTPi_RETURN_ERROR(FTPi_Readline(recv_buf, FTPi_BUFFERSIZE_COMMAND));

    // 応答コマンドを検査
    if (!FTPi_IsSuccess(recv_buf))
    {
        return -1;
    }

    // 複数行の応答の場合、最後まで読み続ける
    if (!FTPi_ReadMoreData(recv_buf))
    {
        return -1;
    }

    result = SOC_Close(FTPi_CommandSocket);

    return 0;
}

/*---------------------------------------------------------------------------*
  Name:         FTPi_Readline

  Description:  コマンドコネクションからから改行まで一行読み込んでバッファに
                コピーする.

  Arguments:    buffer      : バッファへのポインタ
                size        : バッファのサイズ


  Returns:      >= 0なら成功.
 *---------------------------------------------------------------------------*/
int FTPi_Readline(char* buffer, size_t size)
{
    int result;
    int i;
    int buffer_size = 0;

    for (i = 0; i < size; i++)
    {
        result = SOC_Read(FTPi_CommandSocket, buffer + i, 1);
        if ((result == 0) || (buffer[i] == '\n'))
        {
            buffer[i + 1] = '\0';
            break;
        }

        buffer_size++;
    }

    // 受信コマンドの表示
    (void)STD_StrLCpy(buffer, buffer, buffer_size);
    OS_TPrintf("<%s\n", buffer);
    return i;
}

/*---------------------------------------------------------------------------*
  Name:         FTPi_Printf

  Description:  コマンドコネクションに書き込む.

  Arguments:    fmt         : 書き込む文字列


  Returns:      >= 0なら成功.
 *---------------------------------------------------------------------------*/
int FTPi_Printf(const char* fmt, ...)
{
    char    buffer[FTPi_BUFFERSIZE_COMMAND];
    {
        va_list vlist;

        va_start(vlist, fmt);
        (void)OS_VSNPrintf(buffer, FTPi_BUFFERSIZE_COMMAND, fmt, vlist);    // バッファにコピー
        va_end(vlist);
    }
    {
        int result;

        //write処理
        result = SOC_Write(FTPi_CommandSocket, buffer, (int)strlen(buffer));

        // NitroSDK3.2PR以前のSTD_CopyLStringと互換性を保つため
        MI_CpuClear8(buffer, sizeof(buffer));

        // 送信コマンドの表示
        (void)STD_StrLCpy(buffer, buffer, (int)strlen(buffer) - 1);
        OS_TPrintf(">%s\n", buffer);
        return result;
    }
}

/*---------------------------------------------------------------------------*
  Name:         FTPi_GetStatus

  Description:  サーバの応答文字列からステータスコードを得る.

  Arguments:    line        : サーバの応答文字列


  Returns:      ステータスコード.
 *---------------------------------------------------------------------------*/
int FTPi_GetStatus(const char* line)
{
    char    str[5];
    int     recv_command;

    // NitroSDK3.2PR以前のSTD_CopyLStringと互換性を保つため
    MI_CpuClear8(str, sizeof(str));

    // 応答コードを取得
    (void)STD_StrLCpy(str, line, 4);    //STD_
    recv_command = atoi(str);

    return recv_command;
}

/*---------------------------------------------------------------------------*
  Name:         ConnectToFtpServer

  Description:  ソケットを生成しFTPサーバとの接続を試行し完了を待つ.

  Arguments:    http_addr     : 接続先アドレス
                http_port     : 接続先ポート

  Returns:      生成し接続されたソケット( >= 0) あるいはエラー値 ( < 0).
 *---------------------------------------------------------------------------*/
int ConnectToFtpServer(const char* http_addr, u16 http_port)
{
    int         socket;
    int         result;

    SOHostEnt*  host_ent;

    /* ソケット生成 */
    socket = -1;
    result = SOC_Socket(SOC_PF_INET, SOC_SOCK_STREAM, 0);
    if (result < 0)
        OS_TPrintf("failed to create socket\n");
    {
        SOSockAddrIn    sa_in;

        socket = result;

        /* ホスト情報のロード */
        sa_in.len = sizeof(sa_in);
        sa_in.family = IP_INET;
        sa_in.port = SOC_HtoNs(http_port);
        host_ent = SOC_GetHostByName(http_addr);
        MI_CpuCopy8(host_ent->addrList[0], &sa_in.addr, IP_ALEN);

        /* 接続試行 */
        result = SOC_Connect(socket, &sa_in);
        if (result < 0)
            OS_TPrintf("failed to connect\n");
    }

    if ((result < 0) && (socket >= 0))
    {
        result = SOC_Close(socket);
        if (result < 0)
            OS_TPrintf("failed to close socket\n");
    }

    return(result >= 0) ? socket : result;
}

/*---------------------------------------------------------------------------*
  Name:         FTP_TransferType

  Description:  FTPのデータ表現方法をバイナリに指定する.

  Arguments:    type        : TYPEの指定
                              'I' :   バイナリ
                              'A' :   アスキー

  Returns:      >= 0なら成功.
 *---------------------------------------------------------------------------*/
int FTP_TransferType(const char type)
{
    char    recv_buf[FTPi_BUFFERSIZE_COMMAND];

    // FileTypeを選択
    FTPi_RETURN_ERROR(FTPi_Printf("TYPE %c\r\n", type));
    FTPi_RETURN_ERROR(FTPi_Readline(recv_buf, FTPi_BUFFERSIZE_COMMAND));

    // 応答コマンドを検査
    if (!FTPi_IsSuccess(recv_buf))
    {
        return -1;
    }

    // 複数行の応答の場合、最後まで読み続ける
    if (!FTPi_ReadMoreData(recv_buf))
    {
        return -1;
    }

    return 0;
}

/*---------------------------------------------------------------------------*
  Name:         FTPi_ReadMoreData

  Description:  FTPサーバから複数行の応答があった場合、最後まで読み進める.

  Arguments:    line        : 読み込むバッファ

  Returns:      TRUEなら成功.
 *---------------------------------------------------------------------------*/
BOOL FTPi_ReadMoreData(char* line)
{
    int result;

    // 複数行の応答の場合、最後まで読み続ける
    while (FTPi_IsMoreDataAvailable(line))
    {
        result = FTPi_Readline(line, FTPi_BUFFERSIZE_COMMAND);
        if (result <= 0)
        {
            return FALSE;
        }

        // 応答コマンドを検査
        if (!FTPi_IsSuccess(line))
        {
            return FALSE;
        }
    }

    return TRUE;
}

/*---------------------------------------------------------------------------*
  Name:         FTPi_IsMoreDataAvailable

  Description:  FTPサーバから複数行の応答があるかチェック.

  Arguments:    line        : 読み込むバッファ

  Returns:      TRUEなら成功.
 *---------------------------------------------------------------------------*/
BOOL FTPi_IsMoreDataAvailable(const char* line)
{
    if (line[3] == '-')
    {
        return TRUE;
    }

    return FALSE;
}

/*---------------------------------------------------------------------------*
  Name:         FTPi_IsSuccess

  Description:  FTPサーバからの応答コマンドが正常かチェック.

  Arguments:    line        : 応答コマンドの含まれるReadしたバッファ

  Returns:      TRUEなら成功.
 *---------------------------------------------------------------------------*/
BOOL FTPi_IsSuccess(const char* line)
{
    int result;

    // 応答コマンドを検査
    result = FTPi_GetStatus(line);
    if (result >= 400)
    {
        return FALSE;
    }

    return TRUE;
}

/*---------------------------------------------------------------------------*
  Name:         FTP_Upload

  Description:  ファイルのアップロードを行う

  Arguments:    pfiletype    : ファイル転送モード
                pfilesize    : データ転送サイズ
                pfilename    : ファイル名

  Returns:      None.
 *---------------------------------------------------------------------------*/
void FTP_Upload(void* pfiletype, void* pfilesize, void* pfilename)
{
    BOOL        filetype = (BOOL) pfiletype;
    int         filesize = (int)pfilesize;
    const char*     filename = (const char*)pfilename;
    char        cur_key;                        // FTPコマンドの送信要求検知ボタン用
    int     result;
    int     passive_result;

    OSTick  timerStart, timerEnd;               // スループット計測用チック
    u64     transTime;                          // 送受信に掛かった時間
    int     transSize;                          // 送受信したサイズ
    BOOL    abor_flg = FALSE;                   // アップデート中断フラグ
    int     sendSize = FTPi_BUFFERSIZE_DATA;    // 送信サイズ
    int     restSize;               // 送信される残りのサイズ

    // このresultにはデータコネクションのソケットが返ってきている
    passive_result = FTP_Passive(filetype);
    if (passive_result < 0)
        OS_TPrintf("failed in FTP_Passive\n");

    // ファイルデータを表示
    filesize = FTP_OnlyStor(filename);
    if (filesize < 0)
        OS_TPrintf("failed in FTP_OnlyStor\n");

    restSize = filesize;

    // 時間を取得して受信時間を計る
    transTime = 0;                  // 通信時間の初期化
    transSize = 0;                  // 総送信サイズの初期化
    timerStart = OS_GetTick();

    if (passive_result >= 0 && filesize >= 0)
    {
        do
        {
            sendSize = MATH_IMin(restSize, FTPi_BUFFERSIZE_DATA);

            result = FTP_OnlySendData(sendSize);
            transSize += result;    // 送信したファイルのサイズ
            if (filesize <= transSize)
            {
                break;
            }
            else
            {
                cur_key = (char)PAD_Read();
                if (cur_key == PAD_BUTTON_B)
                {
                    // Bボタンが押されたので、受信の途中終了処理を。
                    (void)FTP_OnlyAbor();
                    abor_flg = TRUE;
                    break;
                }
            }

            restSize -= result;
        }
        while (1);

        timerEnd = OS_GetTick();
        transTime = OS_TicksToMilliSeconds((timerEnd - timerStart));

        OS_TPrintf("\nsendTime = %d msec\n", transTime);
        OS_TPrintf("this phase send result = %d byte\n", transSize);
        OS_TPrintf("throughput = %d byte/sec\n\n", transSize * 1000 / transTime);

        // データコネクションを閉じる
        result = FTP_CloseDataSocket();
        if (result < 0)
            OS_TPrintf("failed in FTP_CloseDataSocket\n");
    }

    if (abor_flg == TRUE)
    {
        // ABORに対する受信
        result = FTP_OnlyRecvCommand();
    }
}

/*---------------------------------------------------------------------------*
  Name:         FTP_Download

  Description:  ファイルのダウンロードを行う

  Arguments:    pfiletype    : ファイル転送モード
                pfilesize    : データ転送サイズ
                pfilename    : ファイル名

  Returns:      None.
 *---------------------------------------------------------------------------*/
void FTP_Download(void* pfiletype, void* pfilesize, void* pfilename)
{
    BOOL        filetype = (BOOL) pfiletype;
    int         filesize = (int)pfilesize;
    const char*     filename = (const char*)pfilename;
    char        cur_key;            // FTPコマンドの送信要求検知ボタン用
    int     result;
    int     passive_result;
    int     retr_result;

    OSTick  timerStart, timerEnd;   // スループット計測用チック
    u64     transTime;              // 送受信に掛かった時間
    int     transSize;              // 送受信したサイズ
    BOOL    abor_flg = FALSE;       // アップデート中断フラグ
    int     recvSize = FTPi_BUFFERSIZE_DOWN;    // 受信サイズ
    int     restSize;               // 受信される残りサイズ

    // このresultにはデータコネクションのソケットが返ってきている
    passive_result = FTP_Passive(filetype);
    if (passive_result < 0)
        OS_TPrintf("failed in FTP_Passive\n");

    retr_result = FTP_OnlyRetr(filename);
    if (retr_result < 0)
        OS_TPrintf("failed in FTP_OnlyRetr\n");
    restSize = filesize;

    // 時間を取得して受信時間を計る
    transTime = 0;                  // 通信時間の初期化
    transSize = 0;                  // 受信サイズの初期化
    timerStart = OS_GetTick();

    if (passive_result >= 0 && retr_result >= 0)
    {
        OS_TPrintf("\n--------------------------------\n");
        do
        {
            recvSize = MATH_IMin(restSize, FTPi_BUFFERSIZE_DOWN);
            result = FTP_OnlyRecvData(recvSize, NULL);

            // SOC_EAGAINとSOC_ENOBUFSは負の値だがエラーでないので省く
            if ((result == SOC_EAGAIN) || (result == SOC_ENOBUFS))
                result = 0;
            if (result < 0)
                OS_TPrintf("failed in FTP_OnlyRecvData\n");
            transSize += result;    // 受信したファイルのサイズ
            if (recvSize <= transSize)
            {
                break;
            }
            else
            {
                cur_key = (char)PAD_Read();
                if (cur_key == PAD_BUTTON_B)
                {
                    // Bボタンが押されたので、受信の途中終了処理を。
                    (void)FTP_OnlyAbor();
                    abor_flg = TRUE;
                    break;
                }
            }

            restSize -= result;
        }
        while (1);
        OS_TPrintf("\n--------------------------------\n");

        timerEnd = OS_GetTick();
        transTime = OS_TicksToMilliSeconds((timerEnd - timerStart));

        OS_TPrintf("\nrecvTime = %d msec\n", transTime);
        OS_TPrintf("this phase recv result = %d\n", transSize);
        OS_TPrintf("throughput = %d byte/sec\n\n", transSize * 1000 / transTime);

        // データコネクションを閉じる
        result = FTP_CloseDataSocket();
        if (result < 0)
            OS_TPrintf("failed in FTP_CloseDataSocket\n");
    }

    if (abor_flg == TRUE)
    {
        // ABOR に対する応答コマンドを受け取る
        result = FTP_OnlyRecvCommand();
    }
}

/*---------------------------------------------------------------------------*
  Name:         FTP_Nlst

  Description:  ファイルのリスト表示

  Arguments:    pfiletype    : ファイル転送モード
                pfilesize    : データ転送サイズ(unused)
                pfilename    : ファイル名(unused)

  Returns:      None.
 *---------------------------------------------------------------------------*/
void FTP_Nlst(void* pfiletype, void* pfilesize, void* pfilename)
{
    BOOL    filetype = (BOOL) pfiletype;

    int     result;
    int     passive_result;
    int     send_result;

    (void)pfilesize;
    (void)pfilename;

    // このresultにはデータコネクションのソケットが返ってきている
    passive_result = FTP_Passive(filetype);
    if (passive_result < 0)
        OS_TPrintf("failed in FTP_Passive\n");

    send_result = FTP_OnlySendCommand("NLST -1\r\n");
    if (send_result < 0)
        OS_TPrintf("failed in NLST\n");

    if (passive_result >= 0 && send_result >= 0)
    {
        OS_TPrintf("\n--------------------------------\n");
        while (FTP_OnlyRecvData(FILEDATA256, NULL) != 0)
        {
            // データは全て読み込む
            ;
        }

        OS_TPrintf("\n--------------------------------\n");

        result = FTP_OnlyRecvCommand();
        if (result < 0)
            OS_TPrintf("failed in Command recv\n");

        result = FTP_CloseDataSocket();
        if (result < 0)
            OS_TPrintf("failed in FTP_CloseDataSocket\n");
    }
}

/*---------------------------------------------------------------------------*
  Name:         FTP_Logout

  Description:  ログアウト

  Arguments:    pfiletype    : ファイル転送モード(unused)
                pfilesize    : データ転送サイズ(unused)
                pfilename    : ファイル名(unused)

  Returns:      None.
 *---------------------------------------------------------------------------*/
void FTP_Logout(void* pfiletype, void* pfilesize, void* pfilename)
{
    int result;
    (void)pfiletype;
    (void)pfilesize;
    (void)pfilename;

    result = FTP_Quit();
    if (result < 0)
        OS_TPrintf("failed in FTP_Quit\n");

    OS_Printf("finished ftpclient\n");

    ClearScreen();
}

/*---------------------------------------------------------------------------*
  Name:         FTP_ASCMode

  Description:  ファイルの転送モードをASCIIモードに変更

  Arguments:    pfiletype    : ファイル転送モード
                pfilesize    : データ転送サイズ(unused)
                pfilename    : ファイル名(unused)

  Returns:      None.
 *---------------------------------------------------------------------------*/
void FTP_ASCMode(void* pfiletype, void* pfilesize, void* pfilename)
{
    BOOL    filetype = (BOOL) pfiletype;

    (void)pfilesize;
    (void)pfilename;

    filetype = FALSE;
    OS_TPrintf("\n--------------------------------\n");
    OS_TPrintf("  file type = ASCII\n");
    OS_TPrintf("\n--------------------------------\n");
}

/*---------------------------------------------------------------------------*
  Name:         FTP_BINMode

  Description:  ファイルの転送モードをBINARYモードに変更

  Arguments:    pfiletype    : ファイル転送モード
                pfilesize    : データ転送サイズ(unused)
                pfilename    : ファイル名(unused)

  Returns:      None.
 *---------------------------------------------------------------------------*/
void FTP_BINMode(void* pfiletype, void* pfilesize, void* pfilename)
{
    BOOL    filetype = (BOOL) pfiletype;

    (void)pfilesize;
    (void)pfilename;

    filetype = TRUE;
    OS_TPrintf("\n--------------------------------\n");
    OS_TPrintf("  file type = BINARY\n");
    OS_TPrintf("\n--------------------------------\n");
}

/*---------------------------------------------------------------------------*
  Name:         FTP_Ls

  Description:  ファイルのリスト表示

  Arguments:    pfiletype    : ファイル転送モード(unused)
                pfilesize    : データ転送サイズ(unused)
                pfilename    : ファイル名(unused)

  Returns:      None.
 *---------------------------------------------------------------------------*/
void FTP_Ls(void* pfiletype, void* pfilesize, void* pfilename)
{
    FSFile      d;
    FSDirEntry  e;

    (void)pfiletype;
    (void)pfilesize;
    (void)pfilename;

    FS_InitFile(&d);
    (void)FS_FindDir(&d, "");

    OS_TPrintf("\n--------------------------------\n");
    MI_WriteByte(e.name + 0, (u8) '\0');
    (void)FS_TellDir(&d, &e.dir_id);
    (void)RecursiveEnumDirEx(0, &e, TRUE, NULL);
    OS_TPrintf("\n--------------------------------\n");
}

/*---------------------------------------------------------------------------*
  Name:         FTP_Help

  Description:  ヘルプの表示

  Arguments:    pfiletype    : ファイル転送モード(unused)
                pfilesize    : データ転送サイズ(unused)
                pfilename    : ファイル名(unused)

  Returns:      None.
 *---------------------------------------------------------------------------*/
void FTP_Help(void* pfiletype, void* pfilesize, void* pfilename)
{
    (void)pfiletype;
    (void)pfilesize;
    (void)pfilename;

    OS_TPrintf("\n--------------------------------\n");
    OS_TPrintf("  Push UP       : Cursor Up\n");
    OS_TPrintf("  Push DOWN     : Cursor Down\n");
    OS_TPrintf("  Push A Button : Menu Select\n");
    OS_TPrintf("  Push B Button : Cancel\n");
    OS_TPrintf("\n--------------------------------\n");
}

/*---------------------------------------------------------------------------*
  Name:         FTP_LIST

  Description:  ファイルのリスト表示

  Arguments:    pfiletype            : ファイル転送モード
                pfilename            : LISTコマンドの場合にのみ使用するファイル名
                download_filedata    : ダウンロードするファイルのデータ
                option               FALSE : NLST -1
                                     TRUE  : LIST filename

  Returns:      None.
 *---------------------------------------------------------------------------*/
void FTP_LIST(void* pfiletype, void* pfilename, char download_filedata[FILEDATA256], int option)
{
    BOOL        filetype = (BOOL) pfiletype;

    const char*     filename;
    char        recv_filedata[FILEDATA256];

    int result;
    int passive_result;
    int send_result;

    MI_CpuClear8(recv_filedata, sizeof(recv_filedata));

    if (option)
    {
        filename = (const char*)pfilename;
    }
    else
    {
        (void)pfilename;
    }

    // このresultにはデータコネクションのソケットが返ってきている
    passive_result = FTP_Passive(filetype);
    if (passive_result < 0)
        OS_TPrintf("failed in FTP_Passive\n");

    // optionがTRUEのときはLISTコマンド
    if (option)
    {
        send_result = FTP_OnlySendCommand("LIST %s\r\n", filename);
    }

    // optionがFALSEのときはNLST -1コマンド
    else
    {
        send_result = FTP_OnlySendCommand("NLST -1\r\n");
    }

    if (send_result < 0)
        OS_TPrintf("failed in NLST\n");

    if (passive_result >= 0 && send_result >= 0)
    {
        while (FTP_OnlyRecvData(FILEDATA256, recv_filedata) != 0)
        {
            // データは全て読み込む
            if (recv_filedata[0] != NULL)
            {
                (void)STD_StrCat(download_filedata, recv_filedata);
            }

            MI_CpuClear8(recv_filedata, sizeof(recv_filedata));
        }

        result = FTP_OnlyRecvCommand();
        if (result < 0)
            OS_TPrintf("failed in Command recv\n");

        result = FTP_CloseDataSocket();
        if (result < 0)
            OS_TPrintf("failed in FTP_CloseDataSocket\n");
    }
}

/*---------------------------------------------------------------------------*
  Name:         RecursiveEnumDirEx

  Description:  ディレクトリ内のファイル名をリスト表示、保存します。

  Arguments:    tab                : 字下げ幅
                pe                 : 処理先のディレクトリ、FSDirEntry構造体のアドレス
                output_main        : TUREならそのディレクトリ以下のディレクトリ、ファイル名を表示
                                     FALSEならそのディレクトリのファイル名を保存。
                upload_filename    : ファイル名の保存先

  Returns:      表示または保存したディレクトリ、ファイル数
 *---------------------------------------------------------------------------*/
int RecursiveEnumDirEx(int tab, FSDirEntry* pe, BOOL output_main, char upload_filename[FILEDATA256])
{
    int     line = 0;
    FSFile  d;
    FS_InitFile(&d);

    if (output_main)
        OS_Printf("%*s%s/\n", tab, "", pe->name);
    if (FS_SeekDir(&d, &pe->dir_id))
    {
        tab += 2;
        while (FS_ReadDir(&d, pe))
        {
            if (pe->is_directory)
            {
                if (output_main)
                    (void)RecursiveEnumDirEx(tab, pe, output_main, NULL);
            }
            else
            {
                if (output_main)
                    OS_Printf("%*s%s\n", tab, "", pe->name);
                else
                {
                    PrintString(tab, line + (UPLOAD + 1), COLOR_WHITE, "%s", pe->name);

                    // 区切りのため改行文字を入れる
                    (void)STD_StrCat(pe->name, "\r\n");
                    (void)STD_StrCat(upload_filename, pe->name);
                }
            }

            line++;
        }
    }

    return line;
}

/*---------------------------------------------------------------------------*
  Name:         Upload_fgets

  Description:  アップロードするファイルのデータを取得

  Arguments:    buf    : ファイルデータ格納先バッファへのポインタ
                len    : バッファのサイズ
                p_file : FSFile構造体のアドレス

  Returns:      アップロードするファイルのデータ、NULL なら失敗.
 *---------------------------------------------------------------------------*/
static char* Upload_fgets(char* buf, int len, FSFile* p_file)
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

/*---------------------------------------------------------------------------*/

/* END OF FILE */
