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
  ftp.���̒ǉ�

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

  Description:  FTP�T�[�o�ɐڑ�����.

  Arguments:    http_addr     : �ڑ���A�h���X
                http_port     : �ڑ���|�[�g

  Returns:      >= 0�Ȃ琬��.
 *---------------------------------------------------------------------------*/
int     FTP_Open(const char* http_addr, u16 http_port);

/*---------------------------------------------------------------------------*
  Name:         FTP_Login

  Description:  USER��PASS�R�}���h�𔭍s��FTP�T�[�o�Ƀ��O�C������.

  Arguments:    user_name     : �ڑ���A�J�E���g
                password      : �ڑ���p�X���[�h
                

  Returns:      >= 0�Ȃ琬��.
 *---------------------------------------------------------------------------*/
int     FTP_Login(const char* user_name, const char* password);

/*---------------------------------------------------------------------------*
  Name:         FTP_Passive

  Description:  Passive���[�h�Ńf�[�^�R�l�N�V�������m������.

  Arguments:    filetype �t�@�C���]���̃��[�h TRUE  : BINARY���[�h
                                              FALSE : ASCII���[�h

  Returns:      >= 0�Ȃ琬��.
 *---------------------------------------------------------------------------*/
int     FTP_Passive(BOOL filetype);

/*---------------------------------------------------------------------------*
  Name:         FTP_OnlyStor

  Description:  STOR �R�}���h�݂̂𑗂�

  Arguments:    filename    : �A�b�v���[�h����t�@�C����

  Returns:      >= 0�Ȃ琬��.
 *---------------------------------------------------------------------------*/
int     FTP_OnlyStor(const char* filename);

/*---------------------------------------------------------------------------*
  Name:         FTP_OnlySendData

  Description:  SOC_Send ��񓯊��ň�x�s��

  Arguments:    size    : ���M����T�C�Y.

  Returns:      ����� send �ŃA�b�v���[�h�����T�C�Y.
 *---------------------------------------------------------------------------*/
int     FTP_OnlySendData(int size);

/*---------------------------------------------------------------------------*
  Name:         FTP_OnlyRetr

  Description:  RETR �R�}���h�݂̂𑗂�

  Arguments:    filename    : �_�E�����[�h����t�@�C����

  Returns:      >= 0�Ȃ琬��.
 *---------------------------------------------------------------------------*/
int     FTP_OnlyRetr(const char* filename);

/*---------------------------------------------------------------------------*
  Name:         FTP_OnlyRecvData

  Description:  SOC_Recv ��񓯊��ň�x�s��

  Arguments:    filesize         : �f�[�^�]���T�C�Y
                recv_filename    : NLST�R�}���h�̂Ƃ��A��M�����t�@�C����������

  Returns:      ����� recv �Ń_�E�����[�h�����T�C�Y.
 *---------------------------------------------------------------------------*/
int     FTP_OnlyRecvData(int filesize, char recv_filename[FILEDATA256]);

/*---------------------------------------------------------------------------*
  Name:         FTP_CloseDataSocket

  Description:  FTPi_DataSocket ����ăR�l�N�V������ؒf����

  Arguments:    none.

  Returns:      >=0 �Ȃ琬��.
 *---------------------------------------------------------------------------*/
int     FTP_CloseDataSocket(void);

/*---------------------------------------------------------------------------*
  Name:         FTP_OnlyAbor

  Description:  ABOR �R�}���h�݂̂𑗂�(�������b�Z�[�W�̎�M����s��Ȃ�)

  Arguments:    none.

  Returns:      >= 0�Ȃ琬��.
 *---------------------------------------------------------------------------*/
int     FTP_OnlyAbor();

/*---------------------------------------------------------------------------*
  Name:         FTP_OnlySendCommand

  Description:  FTPi_Printf���s��.

  Arguments:    fmt         : �������ޕ�����


  Returns:      >= 0�Ȃ琬��.
 *---------------------------------------------------------------------------*/
int     FTP_OnlySendCommand(const char* fmt, ...);

/*---------------------------------------------------------------------------*
  Name:         FTP_OnlyRecvCommand

  Description:  FTPi_Readline���s��

  Arguments:    none.

  Returns:      >= 0�Ȃ琬��.
 *---------------------------------------------------------------------------*/
int     FTP_OnlyRecvCommand(void);

/*---------------------------------------------------------------------------*
  Name:         FTP_FileSize

  Description:  �w�肳�ꂽ�t�@�C�����̃T�C�Y�𓾂�

  Arguments:    filename    : ���ׂ�t�@�C����

  Returns:      �t�@�C���̃T�C�Y.
 *---------------------------------------------------------------------------*/
int     FTP_FileSize(const char* filename);

/*---------------------------------------------------------------------------*
  Name:         FTP_Quit

  Description:  FTP�ڑ����I������.

  Arguments:    �Ȃ�

  Returns:      >= 0��_buf) )
    {�琬��.
 *---------------------------------------------------------------------------*/
int     FTP_Quit(void);

/*---------------------------------------------------------------------------*
  Name:         FTP_Upload

  Description:  �t�@�C���̃A�b�v���[�h���s��

  Arguments:    pfiletype    : �t�@�C���]�����[�h
                pfilesize    : �f�[�^�]���T�C�Y
                pfilename    : �t�@�C����

  Returns:      None.
 *---------------------------------------------------------------------------*/
void    FTP_Upload(void* pfiletype, void* pfilesize, void* pfilename);

/*---------------------------------------------------------------------------*
  Name:         FTP_Download

  Description:  �t�@�C���̃_�E�����[�h���s��

  Arguments:    pfiletype    : �t�@�C���]�����[�h
                pfilesize    : �f�[�^�]���T�C�Y
                pfilename    : �t�@�C����

  Returns:      None.
 *---------------------------------------------------------------------------*/
void    FTP_Download(void* pfiletype, void* pfilesize, void* pfilename);

/*---------------------------------------------------------------------------*
  Name:         FTP_Nlst

  Description:  �t�@�C���̃��X�g�\��

  Arguments:    pfiletype    : �t�@�C���]�����[�h
                pfilesize    : �f�[�^�]���T�C�Y(unused)
                pfilename    : �t�@�C����(unused)

  Returns:      None.
 *---------------------------------------------------------------------------*/
void    FTP_Nlst(void* pfiletype, void* pfilesize, void* pfilename);

/*---------------------------------------------------------------------------*
  Name:         FTP_Logout

  Description:  ���O�A�E�g

  Arguments:    pfiletype    : �t�@�C���]�����[�h(unused)
                pfilesize    : �f�[�^�]���T�C�Y(unused)
                pfilename    : �t�@�C����(unused)

  Returns:      None.
 *---------------------------------------------------------------------------*/
void    FTP_Logout(void* pfiletype, void* pfilesize, void* pfilename);

/*---------------------------------------------------------------------------*
  Name:         FTP_ASCMode

  Description:  �t�@�C���̓]�����[�h��ASCII���[�h�ɕύX

  Arguments:    pfiletype    : �t�@�C���]�����[�h
                pfilesize    : �f�[�^�]���T�C�Y(unused)
                pfilename    : �t�@�C����(unused)

  Returns:      None.
 *---------------------------------------------------------------------------*/
void    FTP_ASCMode(void* pfiletype, void* pfilesize, void* pfilename);

/*---------------------------------------------------------------------------*
  Name:         FTP_BINMode

  Description:  �t�@�C���̓]�����[�h��BINARY���[�h�ɕύX

  Arguments:    pfiletype    : �t�@�C���]�����[�h
                pfilesize    : �f�[�^�]���T�C�Y(unused)
                pfilename    : �t�@�C����(unused)

  Returns:      None.
 *---------------------------------------------------------------------------*/
void    FTP_BINMode(void* pfiletype, void* pfilesize, void* pfilename);

/*---------------------------------------------------------------------------*
  Name:         FTP_Ls

  Description:  �t�@�C���̃��X�g�\��

  Arguments:    pfiletype    : �t�@�C���]�����[�h(unused)
                pfilesize    : �f�[�^�]���T�C�Y(unused)
                pfilename    : �t�@�C����(unused)

  Returns:      None.
 *---------------------------------------------------------------------------*/
void    FTP_Ls(void* pfiletype, void* pfilesize, void* pfilename);

/*---------------------------------------------------------------------------*
  Name:         FTP_Help

  Description:  �w���v�̕\��

  Arguments:    pfiletype    : �t�@�C���]�����[�h(unused)
                pfilesize    : �f�[�^�]���T�C�Y(unused)
                pfilename    : �t�@�C����(unused)

  Returns:      None.
 *---------------------------------------------------------------------------*/
void    FTP_Help(void* pfiletype, void* pfilesize, void* pfilename);

/*---------------------------------------------------------------------------*
  Name:         FTP_LIST

  Description:  �t�@�C���̃��X�g�\��

  Arguments:    pfiletype            : �t�@�C���]�����[�h
                pfilename            : LIST�R�}���h�̏ꍇ�ɂ̂ݎg�p����t�@�C����
                download_filedata    : �_�E�����[�h����t�@�C���̃f�[�^
                option               FALSE : NLST -1
                                     TRUE  : LIST filename

  Returns:      None.
 *---------------------------------------------------------------------------*/
void    FTP_LIST(void* pfiletype, void* pfilename, char download_filename[FILEDATA256], int option);

/*---------------------------------------------------------------------------*
  Name:         RecursiveEnumDirEx

  Description:  �f�B���N�g�����̃t�@�C���������X�g�\���A�ۑ����܂��B

  Arguments:    tab                : ��������
                pe                 : ������̃f�B���N�g���AFSDirEntry�\���̂̃A�h���X
                output_main        : TURE�Ȃ炻�̃f�B���N�g���ȉ��̃f�B���N�g���A�t�@�C������\��
                                     FALSE�Ȃ炻�̃f�B���N�g���̃t�@�C������ۑ��B
                upload_filename    : �t�@�C�����̕ۑ���

  Returns:      �\���܂��͕ۑ������f�B���N�g���A�t�@�C����
 *---------------------------------------------------------------------------*/
int     RecursiveEnumDirEx(int tab, FSDirEntry* pe, BOOL output_main, char upload_filename[FILEDATA256]);

#ifdef __cplusplus

} /* extern "C" */
#endif

#endif /* NITROWIFI_SOC_SHARED_FTP_H_ */
