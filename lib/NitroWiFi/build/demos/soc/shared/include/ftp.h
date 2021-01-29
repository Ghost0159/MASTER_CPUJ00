/*---------------------------------------------------------------------------*
  Project:  NitroWiFi - demo - SOC - shared
  File:     ftp.h

  Copyright 2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: ftp.h,v $
  Revision 1.2  2006/03/10 09:22:43  kitase_hirotake
  INDENT SOURCE

  Revision 1.1  2006/01/12 02:16:14  okubata_ryoma
  ftp.ｈの追加

  $NoKeywords: $
 *---------------------------------------------------------------------------*/
#if !defined(NITROWIFI_SOC_SHARED_FTP_H_)
#define NITROWIFI_SOC_SHARED_FTP_H_

#include <nitro.h>
#include <nitroWiFi.h>
#include <stdlib.h>

#include "padkeyboard.h"

#ifdef __cplusplus

extern "C"
{
#endif

/*****************************************************************************/

/* constant */
#define FTPi_BUFFERSIZE_COMMAND 256
#define FTPi_BUFFERSIZE_DATA    1440

#define FTPi_BUFFERSIZE_DOWN    1440
#define FTPi_BUFFERSIZE_UP      14600

#define FTPi_RETURN_ERROR(x) \
    { \
        int CODE = (x); \
        if (CODE < 0) \
            return CODE; \
    }

#define UPLOAD      13
#define DOWNLOAD    14
#define NLST        15
#define QUIT        16
#define ASCII       17
#define BINARY      18
#define LS          19
#define HELP        20

#define FILEDATA16  16
#define FILEDATA256 256

/*****************************************************************************/

/* function */

/*---------------------------------------------------------------------------*
  Name:         FTP_Open

  Description:  FTPサーバに接続する.

  Arguments:    http_addr     : 接続先アドレス
                http_port     : 接続先ポート

  Returns:      >= 0なら成功.
 *---------------------------------------------------------------------------*/
int     FTP_Open(const char* http_addr, u16 http_port);

/*---------------------------------------------------------------------------*
  Name:         FTP_Login

  Description:  USERとPASSコマンドを発行しFTPサーバにログインする.

  Arguments:    user_name     : 接続先アカウント
                password      : 接続先パスワード
                

  Returns:      >= 0なら成功.
 *---------------------------------------------------------------------------*/
int     FTP_Login(const char* user_name, const char* password);

/*---------------------------------------------------------------------------*
  Name:         FTP_Passive

  Description:  Passiveモードでデータコネクションを確立する.

  Arguments:    filetype ファイル転送のモード TRUE  : BINARYモード
                                              FALSE : ASCIIモード

  Returns:      >= 0なら成功.
 *---------------------------------------------------------------------------*/
int     FTP_Passive(BOOL filetype);

/*---------------------------------------------------------------------------*
  Name:         FTP_OnlyStor

  Description:  STOR コマンドのみを送る

  Arguments:    filename    : アップロードするファイル名

  Returns:      >= 0なら成功.
 *---------------------------------------------------------------------------*/
int     FTP_OnlyStor(const char* filename);

/*---------------------------------------------------------------------------*
  Name:         FTP_OnlySendData

  Description:  SOC_Send を非同期で一度行う

  Arguments:    size    : 送信するサイズ.

  Returns:      今回の send でアップロードしたサイズ.
 *---------------------------------------------------------------------------*/
int     FTP_OnlySendData(int size);

/*---------------------------------------------------------------------------*
  Name:         FTP_OnlyRetr

  Description:  RETR コマンドのみを送る

  Arguments:    filename    : ダウンロードするファイル名

  Returns:      >= 0なら成功.
 *---------------------------------------------------------------------------*/
int     FTP_OnlyRetr(const char* filename);

/*---------------------------------------------------------------------------*
  Name:         FTP_OnlyRecvData

  Description:  SOC_Recv を非同期で一度行う

  Arguments:    filesize         : データ転送サイズ
                recv_filename    : NLSTコマンドのとき、受信したファイル名を入れる

  Returns:      今回の recv でダウンロードしたサイズ.
 *---------------------------------------------------------------------------*/
int     FTP_OnlyRecvData(int filesize, char recv_filename[FILEDATA256]);

/*---------------------------------------------------------------------------*
  Name:         FTP_CloseDataSocket

  Description:  FTPi_DataSocket を閉じてコネクションを切断する

  Arguments:    none.

  Returns:      >=0 なら成功.
 *---------------------------------------------------------------------------*/
int     FTP_CloseDataSocket(void);

/*---------------------------------------------------------------------------*
  Name:         FTP_OnlyAbor

  Description:  ABOR コマンドのみを送る(応答メッセージの受信すら行わない)

  Arguments:    none.

  Returns:      >= 0なら成功.
 *---------------------------------------------------------------------------*/
int     FTP_OnlyAbor();

/*---------------------------------------------------------------------------*
  Name:         FTP_OnlySendCommand

  Description:  FTPi_Printfを行う.

  Arguments:    fmt         : 書き込む文字列


  Returns:      >= 0なら成功.
 *---------------------------------------------------------------------------*/
int     FTP_OnlySendCommand(const char* fmt, ...);

/*---------------------------------------------------------------------------*
  Name:         FTP_OnlyRecvCommand

  Description:  FTPi_Readlineを行う

  Arguments:    none.

  Returns:      >= 0なら成功.
 *---------------------------------------------------------------------------*/
int     FTP_OnlyRecvCommand(void);

/*---------------------------------------------------------------------------*
  Name:         FTP_FileSize

  Description:  指定されたファイル名のサイズを得る

  Arguments:    filename    : 調べるファイル名

  Returns:      ファイルのサイズ.
 *---------------------------------------------------------------------------*/
int     FTP_FileSize(const char* filename);

/*---------------------------------------------------------------------------*
  Name:         FTP_Quit

  Description:  FTP接続を終了する.

  Arguments:    なし

  Returns:      >= 0な_buf) )
    {ら成功.
 *---------------------------------------------------------------------------*/
int     FTP_Quit(void);

/*---------------------------------------------------------------------------*
  Name:         FTP_Upload

  Description:  ファイルのアップロードを行う

  Arguments:    pfiletype    : ファイル転送モード
                pfilesize    : データ転送サイズ
                pfilename    : ファイル名

  Returns:      None.
 *---------------------------------------------------------------------------*/
void    FTP_Upload(void* pfiletype, void* pfilesize, void* pfilename);

/*---------------------------------------------------------------------------*
  Name:         FTP_Download

  Description:  ファイルのダウンロードを行う

  Arguments:    pfiletype    : ファイル転送モード
                pfilesize    : データ転送サイズ
                pfilename    : ファイル名

  Returns:      None.
 *---------------------------------------------------------------------------*/
void    FTP_Download(void* pfiletype, void* pfilesize, void* pfilename);

/*---------------------------------------------------------------------------*
  Name:         FTP_Nlst

  Description:  ファイルのリスト表示

  Arguments:    pfiletype    : ファイル転送モード
                pfilesize    : データ転送サイズ(unused)
                pfilename    : ファイル名(unused)

  Returns:      None.
 *---------------------------------------------------------------------------*/
void    FTP_Nlst(void* pfiletype, void* pfilesize, void* pfilename);

/*---------------------------------------------------------------------------*
  Name:         FTP_Logout

  Description:  ログアウト

  Arguments:    pfiletype    : ファイル転送モード(unused)
                pfilesize    : データ転送サイズ(unused)
                pfilename    : ファイル名(unused)

  Returns:      None.
 *---------------------------------------------------------------------------*/
void    FTP_Logout(void* pfiletype, void* pfilesize, void* pfilename);

/*---------------------------------------------------------------------------*
  Name:         FTP_ASCMode

  Description:  ファイルの転送モードをASCIIモードに変更

  Arguments:    pfiletype    : ファイル転送モード
                pfilesize    : データ転送サイズ(unused)
                pfilename    : ファイル名(unused)

  Returns:      None.
 *---------------------------------------------------------------------------*/
void    FTP_ASCMode(void* pfiletype, void* pfilesize, void* pfilename);

/*---------------------------------------------------------------------------*
  Name:         FTP_BINMode

  Description:  ファイルの転送モードをBINARYモードに変更

  Arguments:    pfiletype    : ファイル転送モード
                pfilesize    : データ転送サイズ(unused)
                pfilename    : ファイル名(unused)

  Returns:      None.
 *---------------------------------------------------------------------------*/
void    FTP_BINMode(void* pfiletype, void* pfilesize, void* pfilename);

/*---------------------------------------------------------------------------*
  Name:         FTP_Ls

  Description:  ファイルのリスト表示

  Arguments:    pfiletype    : ファイル転送モード(unused)
                pfilesize    : データ転送サイズ(unused)
                pfilename    : ファイル名(unused)

  Returns:      None.
 *---------------------------------------------------------------------------*/
void    FTP_Ls(void* pfiletype, void* pfilesize, void* pfilename);

/*---------------------------------------------------------------------------*
  Name:         FTP_Help

  Description:  ヘルプの表示

  Arguments:    pfiletype    : ファイル転送モード(unused)
                pfilesize    : データ転送サイズ(unused)
                pfilename    : ファイル名(unused)

  Returns:      None.
 *---------------------------------------------------------------------------*/
void    FTP_Help(void* pfiletype, void* pfilesize, void* pfilename);

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
void    FTP_LIST(void* pfiletype, void* pfilename, char download_filename[FILEDATA256], int option);

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
int     RecursiveEnumDirEx(int tab, FSDirEntry* pe, BOOL output_main, char upload_filename[FILEDATA256]);

#ifdef __cplusplus

} /* extern "C" */
#endif

#endif /* NITROWIFI_SOC_SHARED_FTP_H_ */
