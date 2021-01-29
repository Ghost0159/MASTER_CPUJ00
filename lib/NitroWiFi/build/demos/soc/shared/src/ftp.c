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
  NitroSDK3.2PR�ȑO��STD_CopyLString�ƌ݊�����ۂ��߂̕ύX

  Revision 1.4  2006/07/18 00:42:08  okubata_ryoma
  small fix

  Revision 1.3  2006/07/18 00:06:14  okubata_ryoma
  STD_CopyLString�̎d�l�ύX�ɂ��ύX

  Revision 1.2  2006/03/10 09:22:43  kitase_hirotake
  INDENT SOURCE

  Revision 1.1  2006/01/12 02:16:41  okubata_ryoma
  ftp.���̒ǉ�


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

  Description:  FTP�T�[�o�ɐڑ�����.

  Arguments:    http_addr     : �ڑ���A�h���X
                http_port     : �ڑ���|�[�g

  Returns:      >= 0�Ȃ琬��.
 *---------------------------------------------------------------------------*/
int FTP_Open(const char* http_addr, u16 http_port)
{
    // �T�[�o�ɃR�}���h�p�̃R�l�N�V�����𒣂�
    FTPi_CommandSocket = ConnectToFtpServer(http_addr, http_port);

    return FTPi_CommandSocket;
}

/*---------------------------------------------------------------------------*
  Name:         FTP_Login

  Description:  USER��PASS�R�}���h�𔭍s��FTP�T�[�o�Ƀ��O�C������.

  Arguments:    user_name     : �ڑ���A�J�E���g
                password      : �ڑ���p�X���[�h
                

  Returns:      >= 0�Ȃ琬��.
 *---------------------------------------------------------------------------*/
int FTP_Login(const char* user_name, const char* password)
{
    char    recv_buf[FTPi_BUFFERSIZE_COMMAND];

    // Connect�ɑ΂��郁�b�Z�[�W����M
    FTPi_RETURN_ERROR(FTPi_Readline(recv_buf, FTPi_BUFFERSIZE_COMMAND));

    // �����R�}���h������
    if (!FTPi_IsSuccess(recv_buf))
    {
        return -1;
    }

    // �����s�̉����̏ꍇ�A�Ō�܂œǂݑ�����
    if (!FTPi_ReadMoreData(recv_buf))
    {
        return -1;
    }

    // UserName
    FTPi_RETURN_ERROR(FTPi_Printf("USER %s\r\n", user_name));
    FTPi_RETURN_ERROR(FTPi_Readline(recv_buf, FTPi_BUFFERSIZE_COMMAND));

    // �����R�}���h������
    if (!FTPi_IsSuccess(recv_buf))
    {
        return -1;
    }

    // �����s�̉����̏ꍇ�A�Ō�܂œǂݑ�����
    if (!FTPi_ReadMoreData(recv_buf))
    {
        return -1;
    }

    // Password
    FTPi_RETURN_ERROR(FTPi_Printf("PASS %s\r\n", password));
    FTPi_RETURN_ERROR(FTPi_Readline(recv_buf, FTPi_BUFFERSIZE_COMMAND));

    // �����R�}���h������
    if (!FTPi_IsSuccess(recv_buf))
    {
        return -1;
    }

    // �����s�̉����̏ꍇ�A�Ō�܂œǂݑ�����
    if (!FTPi_ReadMoreData(recv_buf))
    {
        return -1;
    }

    return 0;
}

/*---------------------------------------------------------------------------*
  Name:         FTP_Passive

  Description:  Passive���[�h�Ńf�[�^�R�l�N�V�������m������.

  Arguments:    filetype �t�@�C���]���̃��[�h TRUE  : BINARY���[�h
                                              FALSE : ASCII���[�h

  Returns:      >= 0�Ȃ琬��.
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

    // Passive���[�h
    FTPi_RETURN_ERROR(FTPi_Printf("PASV\r\n"));
    FTPi_RETURN_ERROR(FTPi_Readline(recv_buf, FTPi_BUFFERSIZE_COMMAND));

    // �����R�}���h������
    if (!FTPi_IsSuccess(recv_buf))
    {
        return -1;
    }

    // �����s�̉����̏ꍇ�A�Ō�܂œǂݑ�����
    if (!FTPi_ReadMoreData(recv_buf))
    {
        return -1;
    }

    // PASV�R�}���h�̉������b�Z�[�W����T�[�o�̃A�h���X�A�|�[�g�𔲂��o��
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
            int count = 0;  // ','�̏o����
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

        // �T�[�o�Ƀf�[�^�p�̃R�l�N�V�����𒣂�
        FTPi_DataSocket = ConnectToFtpServer(http_addr, http_port);
    }

    return FTPi_DataSocket;
}

/*---------------------------------------------------------------------------*
  Name:         FTP_OnlyStor

  Description:  �A�b�v���[�h����t�@�C���T�C�Y���擾���āASTOR �R�}���h�𑗂�

  Arguments:    filename    : �A�b�v���[�h����t�@�C����

  Returns:      �A�b�v���[�h����t�@�C���̃T�C�Y�A>= 0�Ȃ琬��.
 *---------------------------------------------------------------------------*/
int FTP_OnlyStor(const char* filename)
{
    char    recv_buf[FTPi_BUFFERSIZE_COMMAND];
    int     buf_len;    // �t�@�C�����̕�����̒���
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

    // RETR �R�}���h���M
    FTPi_RETURN_ERROR(FTPi_Printf("STOR %s\r\n", filename));
    FTPi_RETURN_ERROR(FTPi_Readline(recv_buf, FTPi_BUFFERSIZE_COMMAND));

    // �����R�}���h������
    if (!FTPi_IsSuccess(recv_buf))
    {
        return -1;
    }

    // �����s�̉����̏ꍇ�A�Ō�܂œǂݑ�����
    if (!FTPi_ReadMoreData(recv_buf))
    {
        return -1;
    }

    return (int)open_filesize;
}

/*---------------------------------------------------------------------------*
  Name:         FTP_OnlySendData

  Description:  SOC_Send ��񓯊��ň�x�s��

  Arguments:    size    : ���M����T�C�Y.

  Returns:      ����� send �ŃA�b�v���[�h�����T�C�Y.
 *---------------------------------------------------------------------------*/
int FTP_OnlySendData(int size)
{
    int result;

    size = MATH_IMin(size, FTPi_BUFFERSIZE_UP);

    result = SOC_Send(FTPi_DataSocket, send_buf, size, SOC_MSG_DONTWAIT);

    // SOC_EAGAIN��SOC_ENOBUFS�͕��̒l�����G���[�łȂ��̂ŏȂ�
    if ((result != SOC_EAGAIN) && (result != SOC_ENOBUFS))
    {
        FTPi_RETURN_ERROR(result);
        return (int)result;
    }

    return 0;
}

/*---------------------------------------------------------------------------*
  Name:         FTP_OnlyRetr

  Description:  RETR �R�}���h�݂̂𑗂�

  Arguments:    filename    : �_�E�����[�h����t�@�C����

  Returns:      >= 0�Ȃ琬��.
 *---------------------------------------------------------------------------*/
int FTP_OnlyRetr(const char* filename)
{
    char    recv_buf[FTPi_BUFFERSIZE_COMMAND];

    // RETR �R�}���h���M
    FTPi_RETURN_ERROR(FTPi_Printf("RETR %s\r\n", filename));
    FTPi_RETURN_ERROR(FTPi_Readline(recv_buf, FTPi_BUFFERSIZE_COMMAND));

    // �����R�}���h������
    if (!FTPi_IsSuccess(recv_buf))
    {
        return -1;
    }

    // �����s�̉����̏ꍇ�A�Ō�܂œǂݑ�����
    if (!FTPi_ReadMoreData(recv_buf))
    {
        return -1;
    }

    return 0;
}

/*---------------------------------------------------------------------------*
  Name:         FTP_OnlyRecvData

  Description:  SOC_Recv ��񓯊��ň�x�s��

  Arguments:    filesize             : �f�[�^�]���T�C�Y
                recv_filename        : NLST�R�}���h�̂Ƃ��A��M�����t�@�C����������

  Returns:      ����� recv �Ń_�E�����[�h�����T�C�Y.
 *---------------------------------------------------------------------------*/
int FTP_OnlyRecvData(int filesize, char recv_filename[FILEDATA256])
{
    char    recv_buf[FILEDATA256];
    char    print_buf[FILEDATA256];
    char*   return_point;
    int     result;
    char*   precv_buf;
    int     filename_len;

    // NitroSDK3.2PR�ȑO��STD_CopyLString�ƌ݊�����ۂ���
    MI_CpuClear8(print_buf, sizeof(print_buf));

    MI_CpuClear8(recv_buf, FILEDATA256);    // �o�b�t�@��������
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

    // SOC_EAGAIN��SOC_ENOBUFS�͕��̒l�����G���[�łȂ��̂ŏȂ�
    if ((result != SOC_EAGAIN) && (result != SOC_ENOBUFS))
    {
        FTPi_RETURN_ERROR(result);
        return (int)result;
    }

    return result;
}

/*---------------------------------------------------------------------------*
  Name:         FTP_CloseDataSocket

  Description:  FTPi_DataSocket ����ăR�l�N�V������ؒf����

  Arguments:    none.

  Returns:      >=0 �Ȃ琬��.
 *---------------------------------------------------------------------------*/
int FTP_CloseDataSocket(void)
{
    char    recv_buf[FTPi_BUFFERSIZE_COMMAND];
    int     result;

    // �f�[�^�p�\�P�b�g����āA�R�l�N�V������ؒf����
    result = SOC_Close(FTPi_DataSocket);

    FTPi_DataSocket = -1;
    FTPi_RETURN_ERROR(FTPi_Readline(recv_buf, FTPi_BUFFERSIZE_COMMAND));

    // �����R�}���h������
    if (!FTPi_IsSuccess(recv_buf))
    {
        return -1;
    }

    // �����s�̉����̏ꍇ�A�Ō�܂œǂݑ�����
    if (!FTPi_ReadMoreData(recv_buf))
    {
        return -1;
    }

    return 0;
}

/*---------------------------------------------------------------------------*
  Name:         FTP_OnlyAbor

  Description:  ABOR �R�}���h�݂̂𑗂�(�������b�Z�[�W�̎�M����s��Ȃ�)

  Arguments:    none.

  Returns:      >= 0�Ȃ琬��.
 *---------------------------------------------------------------------------*/
int FTP_OnlyAbor()
{
    // RETR �R�}���h���M
    FTPi_RETURN_ERROR(FTPi_Printf("ABOR\r\n"));

    return 0;
}

/*---------------------------------------------------------------------------*
  Name:         FTP_OnlySendCommand

  Description:  FTPi_Printf���s��.

  Arguments:    fmt         : �������ޕ�����


  Returns:      >= 0�Ȃ琬��.
 *---------------------------------------------------------------------------*/
int FTP_OnlySendCommand(const char* fmt, ...)
{
    // RETR �R�}���h���M
    FTPi_RETURN_ERROR(FTPi_Printf(fmt));

    return 0;
}

/*---------------------------------------------------------------------------*
  Name:         FTP_OnlyRecvCommand

  Description:  FTPi_Readline���s��

  Arguments:    none.

  Returns:      >= 0�Ȃ琬��.
 *---------------------------------------------------------------------------*/
int FTP_OnlyRecvCommand(void)
{
    char    recv_buf[FTPi_BUFFERSIZE_COMMAND];

    FTPi_RETURN_ERROR(FTPi_Readline(recv_buf, FTPi_BUFFERSIZE_COMMAND));

    // �����R�}���h������
    if (!FTPi_IsSuccess(recv_buf))
    {
        return -1;
    }

    // �����s�̉����̏ꍇ�A�Ō�܂œǂݑ�����
    if (!FTPi_ReadMoreData(recv_buf))
    {
        return -1;
    }

    return 0;
}

/*---------------------------------------------------------------------------*
  Name:         FTP_FileSize

  Description:  �w�肳�ꂽ�t�@�C�����̃T�C�Y�𓾂�

  Arguments:    filename    : ���ׂ�t�@�C����

  Returns:      �t�@�C���̃T�C�Y.
 *---------------------------------------------------------------------------*/
int FTP_FileSize(const char* filename)
{
    char    recv_buf[FTPi_BUFFERSIZE_COMMAND];
    char*   tmp_ptr;

    // Download����t�@�C���̃T�C�Y�m�F
    FTPi_RETURN_ERROR(FTPi_Printf("SIZE %s\r\n", filename));
    FTPi_RETURN_ERROR(FTPi_Readline(recv_buf, FTPi_BUFFERSIZE_COMMAND));

    // �����R�}���h������
    if (!FTPi_IsSuccess(recv_buf))
    {
        return -1;
    }

    // �����s�̉����̏ꍇ�A�Ō�܂œǂݑ�����
    if (!FTPi_ReadMoreData(recv_buf))
    {
        return -1;
    }

    // SIZE�R�}���h�̉������b�Z�[�W����DownLoad����t�@�C���̃T�C�Y���擾
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

  Description:  FTP�ڑ����I������.

  Arguments:    �Ȃ�

  Returns:      >= 0�Ȃ琬��.
 *---------------------------------------------------------------------------*/
int FTP_Quit(void)
{
    char    recv_buf[FTPi_BUFFERSIZE_COMMAND];
    int     result;

    FTPi_RETURN_ERROR(FTPi_Printf("QUIT\r\n"));
    FTPi_RETURN_ERROR(FTPi_Readline(recv_buf, FTPi_BUFFERSIZE_COMMAND));

    // �����R�}���h������
    if (!FTPi_IsSuccess(recv_buf))
    {
        return -1;
    }

    // �����s�̉����̏ꍇ�A�Ō�܂œǂݑ�����
    if (!FTPi_ReadMoreData(recv_buf))
    {
        return -1;
    }

    result = SOC_Close(FTPi_CommandSocket);

    return 0;
}

/*---------------------------------------------------------------------------*
  Name:         FTPi_Readline

  Description:  �R�}���h�R�l�N�V�������炩����s�܂ň�s�ǂݍ���Ńo�b�t�@��
                �R�s�[����.

  Arguments:    buffer      : �o�b�t�@�ւ̃|�C���^
                size        : �o�b�t�@�̃T�C�Y


  Returns:      >= 0�Ȃ琬��.
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

    // ��M�R�}���h�̕\��
    (void)STD_StrLCpy(buffer, buffer, buffer_size);
    OS_TPrintf("<%s\n", buffer);
    return i;
}

/*---------------------------------------------------------------------------*
  Name:         FTPi_Printf

  Description:  �R�}���h�R�l�N�V�����ɏ�������.

  Arguments:    fmt         : �������ޕ�����


  Returns:      >= 0�Ȃ琬��.
 *---------------------------------------------------------------------------*/
int FTPi_Printf(const char* fmt, ...)
{
    char    buffer[FTPi_BUFFERSIZE_COMMAND];
    {
        va_list vlist;

        va_start(vlist, fmt);
        (void)OS_VSNPrintf(buffer, FTPi_BUFFERSIZE_COMMAND, fmt, vlist);    // �o�b�t�@�ɃR�s�[
        va_end(vlist);
    }
    {
        int result;

        //write����
        result = SOC_Write(FTPi_CommandSocket, buffer, (int)strlen(buffer));

        // NitroSDK3.2PR�ȑO��STD_CopyLString�ƌ݊�����ۂ���
        MI_CpuClear8(buffer, sizeof(buffer));

        // ���M�R�}���h�̕\��
        (void)STD_StrLCpy(buffer, buffer, (int)strlen(buffer) - 1);
        OS_TPrintf(">%s\n", buffer);
        return result;
    }
}

/*---------------------------------------------------------------------------*
  Name:         FTPi_GetStatus

  Description:  �T�[�o�̉��������񂩂�X�e�[�^�X�R�[�h�𓾂�.

  Arguments:    line        : �T�[�o�̉���������


  Returns:      �X�e�[�^�X�R�[�h.
 *---------------------------------------------------------------------------*/
int FTPi_GetStatus(const char* line)
{
    char    str[5];
    int     recv_command;

    // NitroSDK3.2PR�ȑO��STD_CopyLString�ƌ݊�����ۂ���
    MI_CpuClear8(str, sizeof(str));

    // �����R�[�h���擾
    (void)STD_StrLCpy(str, line, 4);    //STD_
    recv_command = atoi(str);

    return recv_command;
}

/*---------------------------------------------------------------------------*
  Name:         ConnectToFtpServer

  Description:  �\�P�b�g�𐶐���FTP�T�[�o�Ƃ̐ڑ������s��������҂�.

  Arguments:    http_addr     : �ڑ���A�h���X
                http_port     : �ڑ���|�[�g

  Returns:      �������ڑ����ꂽ�\�P�b�g( >= 0) ���邢�̓G���[�l ( < 0).
 *---------------------------------------------------------------------------*/
int ConnectToFtpServer(const char* http_addr, u16 http_port)
{
    int         socket;
    int         result;

    SOHostEnt*  host_ent;

    /* �\�P�b�g���� */
    socket = -1;
    result = SOC_Socket(SOC_PF_INET, SOC_SOCK_STREAM, 0);
    if (result < 0)
        OS_TPrintf("failed to create socket\n");
    {
        SOSockAddrIn    sa_in;

        socket = result;

        /* �z�X�g���̃��[�h */
        sa_in.len = sizeof(sa_in);
        sa_in.family = IP_INET;
        sa_in.port = SOC_HtoNs(http_port);
        host_ent = SOC_GetHostByName(http_addr);
        MI_CpuCopy8(host_ent->addrList[0], &sa_in.addr, IP_ALEN);

        /* �ڑ����s */
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

  Description:  FTP�̃f�[�^�\�����@���o�C�i���Ɏw�肷��.

  Arguments:    type        : TYPE�̎w��
                              'I' :   �o�C�i��
                              'A' :   �A�X�L�[

  Returns:      >= 0�Ȃ琬��.
 *---------------------------------------------------------------------------*/
int FTP_TransferType(const char type)
{
    char    recv_buf[FTPi_BUFFERSIZE_COMMAND];

    // FileType��I��
    FTPi_RETURN_ERROR(FTPi_Printf("TYPE %c\r\n", type));
    FTPi_RETURN_ERROR(FTPi_Readline(recv_buf, FTPi_BUFFERSIZE_COMMAND));

    // �����R�}���h������
    if (!FTPi_IsSuccess(recv_buf))
    {
        return -1;
    }

    // �����s�̉����̏ꍇ�A�Ō�܂œǂݑ�����
    if (!FTPi_ReadMoreData(recv_buf))
    {
        return -1;
    }

    return 0;
}

/*---------------------------------------------------------------------------*
  Name:         FTPi_ReadMoreData

  Description:  FTP�T�[�o���畡���s�̉������������ꍇ�A�Ō�܂œǂݐi�߂�.

  Arguments:    line        : �ǂݍ��ރo�b�t�@

  Returns:      TRUE�Ȃ琬��.
 *---------------------------------------------------------------------------*/
BOOL FTPi_ReadMoreData(char* line)
{
    int result;

    // �����s�̉����̏ꍇ�A�Ō�܂œǂݑ�����
    while (FTPi_IsMoreDataAvailable(line))
    {
        result = FTPi_Readline(line, FTPi_BUFFERSIZE_COMMAND);
        if (result <= 0)
        {
            return FALSE;
        }

        // �����R�}���h������
        if (!FTPi_IsSuccess(line))
        {
            return FALSE;
        }
    }

    return TRUE;
}

/*---------------------------------------------------------------------------*
  Name:         FTPi_IsMoreDataAvailable

  Description:  FTP�T�[�o���畡���s�̉��������邩�`�F�b�N.

  Arguments:    line        : �ǂݍ��ރo�b�t�@

  Returns:      TRUE�Ȃ琬��.
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

  Description:  FTP�T�[�o����̉����R�}���h�����킩�`�F�b�N.

  Arguments:    line        : �����R�}���h�̊܂܂��Read�����o�b�t�@

  Returns:      TRUE�Ȃ琬��.
 *---------------------------------------------------------------------------*/
BOOL FTPi_IsSuccess(const char* line)
{
    int result;

    // �����R�}���h������
    result = FTPi_GetStatus(line);
    if (result >= 400)
    {
        return FALSE;
    }

    return TRUE;
}

/*---------------------------------------------------------------------------*
  Name:         FTP_Upload

  Description:  �t�@�C���̃A�b�v���[�h���s��

  Arguments:    pfiletype    : �t�@�C���]�����[�h
                pfilesize    : �f�[�^�]���T�C�Y
                pfilename    : �t�@�C����

  Returns:      None.
 *---------------------------------------------------------------------------*/
void FTP_Upload(void* pfiletype, void* pfilesize, void* pfilename)
{
    BOOL        filetype = (BOOL) pfiletype;
    int         filesize = (int)pfilesize;
    const char*     filename = (const char*)pfilename;
    char        cur_key;                        // FTP�R�}���h�̑��M�v�����m�{�^���p
    int     result;
    int     passive_result;

    OSTick  timerStart, timerEnd;               // �X���[�v�b�g�v���p�`�b�N
    u64     transTime;                          // ����M�Ɋ|����������
    int     transSize;                          // ����M�����T�C�Y
    BOOL    abor_flg = FALSE;                   // �A�b�v�f�[�g���f�t���O
    int     sendSize = FTPi_BUFFERSIZE_DATA;    // ���M�T�C�Y
    int     restSize;               // ���M�����c��̃T�C�Y

    // ����result�ɂ̓f�[�^�R�l�N�V�����̃\�P�b�g���Ԃ��Ă��Ă���
    passive_result = FTP_Passive(filetype);
    if (passive_result < 0)
        OS_TPrintf("failed in FTP_Passive\n");

    // �t�@�C���f�[�^��\��
    filesize = FTP_OnlyStor(filename);
    if (filesize < 0)
        OS_TPrintf("failed in FTP_OnlyStor\n");

    restSize = filesize;

    // ���Ԃ��擾���Ď�M���Ԃ��v��
    transTime = 0;                  // �ʐM���Ԃ̏�����
    transSize = 0;                  // �����M�T�C�Y�̏�����
    timerStart = OS_GetTick();

    if (passive_result >= 0 && filesize >= 0)
    {
        do
        {
            sendSize = MATH_IMin(restSize, FTPi_BUFFERSIZE_DATA);

            result = FTP_OnlySendData(sendSize);
            transSize += result;    // ���M�����t�@�C���̃T�C�Y
            if (filesize <= transSize)
            {
                break;
            }
            else
            {
                cur_key = (char)PAD_Read();
                if (cur_key == PAD_BUTTON_B)
                {
                    // B�{�^���������ꂽ�̂ŁA��M�̓r���I���������B
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

        // �f�[�^�R�l�N�V���������
        result = FTP_CloseDataSocket();
        if (result < 0)
            OS_TPrintf("failed in FTP_CloseDataSocket\n");
    }

    if (abor_flg == TRUE)
    {
        // ABOR�ɑ΂����M
        result = FTP_OnlyRecvCommand();
    }
}

/*---------------------------------------------------------------------------*
  Name:         FTP_Download

  Description:  �t�@�C���̃_�E�����[�h���s��

  Arguments:    pfiletype    : �t�@�C���]�����[�h
                pfilesize    : �f�[�^�]���T�C�Y
                pfilename    : �t�@�C����

  Returns:      None.
 *---------------------------------------------------------------------------*/
void FTP_Download(void* pfiletype, void* pfilesize, void* pfilename)
{
    BOOL        filetype = (BOOL) pfiletype;
    int         filesize = (int)pfilesize;
    const char*     filename = (const char*)pfilename;
    char        cur_key;            // FTP�R�}���h�̑��M�v�����m�{�^���p
    int     result;
    int     passive_result;
    int     retr_result;

    OSTick  timerStart, timerEnd;   // �X���[�v�b�g�v���p�`�b�N
    u64     transTime;              // ����M�Ɋ|����������
    int     transSize;              // ����M�����T�C�Y
    BOOL    abor_flg = FALSE;       // �A�b�v�f�[�g���f�t���O
    int     recvSize = FTPi_BUFFERSIZE_DOWN;    // ��M�T�C�Y
    int     restSize;               // ��M�����c��T�C�Y

    // ����result�ɂ̓f�[�^�R�l�N�V�����̃\�P�b�g���Ԃ��Ă��Ă���
    passive_result = FTP_Passive(filetype);
    if (passive_result < 0)
        OS_TPrintf("failed in FTP_Passive\n");

    retr_result = FTP_OnlyRetr(filename);
    if (retr_result < 0)
        OS_TPrintf("failed in FTP_OnlyRetr\n");
    restSize = filesize;

    // ���Ԃ��擾���Ď�M���Ԃ��v��
    transTime = 0;                  // �ʐM���Ԃ̏�����
    transSize = 0;                  // ��M�T�C�Y�̏�����
    timerStart = OS_GetTick();

    if (passive_result >= 0 && retr_result >= 0)
    {
        OS_TPrintf("\n--------------------------------\n");
        do
        {
            recvSize = MATH_IMin(restSize, FTPi_BUFFERSIZE_DOWN);
            result = FTP_OnlyRecvData(recvSize, NULL);

            // SOC_EAGAIN��SOC_ENOBUFS�͕��̒l�����G���[�łȂ��̂ŏȂ�
            if ((result == SOC_EAGAIN) || (result == SOC_ENOBUFS))
                result = 0;
            if (result < 0)
                OS_TPrintf("failed in FTP_OnlyRecvData\n");
            transSize += result;    // ��M�����t�@�C���̃T�C�Y
            if (recvSize <= transSize)
            {
                break;
            }
            else
            {
                cur_key = (char)PAD_Read();
                if (cur_key == PAD_BUTTON_B)
                {
                    // B�{�^���������ꂽ�̂ŁA��M�̓r���I���������B
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

        // �f�[�^�R�l�N�V���������
        result = FTP_CloseDataSocket();
        if (result < 0)
            OS_TPrintf("failed in FTP_CloseDataSocket\n");
    }

    if (abor_flg == TRUE)
    {
        // ABOR �ɑ΂��鉞���R�}���h���󂯎��
        result = FTP_OnlyRecvCommand();
    }
}

/*---------------------------------------------------------------------------*
  Name:         FTP_Nlst

  Description:  �t�@�C���̃��X�g�\��

  Arguments:    pfiletype    : �t�@�C���]�����[�h
                pfilesize    : �f�[�^�]���T�C�Y(unused)
                pfilename    : �t�@�C����(unused)

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

    // ����result�ɂ̓f�[�^�R�l�N�V�����̃\�P�b�g���Ԃ��Ă��Ă���
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
            // �f�[�^�͑S�ēǂݍ���
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

  Description:  ���O�A�E�g

  Arguments:    pfiletype    : �t�@�C���]�����[�h(unused)
                pfilesize    : �f�[�^�]���T�C�Y(unused)
                pfilename    : �t�@�C����(unused)

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

  Description:  �t�@�C���̓]�����[�h��ASCII���[�h�ɕύX

  Arguments:    pfiletype    : �t�@�C���]�����[�h
                pfilesize    : �f�[�^�]���T�C�Y(unused)
                pfilename    : �t�@�C����(unused)

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

  Description:  �t�@�C���̓]�����[�h��BINARY���[�h�ɕύX

  Arguments:    pfiletype    : �t�@�C���]�����[�h
                pfilesize    : �f�[�^�]���T�C�Y(unused)
                pfilename    : �t�@�C����(unused)

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

  Description:  �t�@�C���̃��X�g�\��

  Arguments:    pfiletype    : �t�@�C���]�����[�h(unused)
                pfilesize    : �f�[�^�]���T�C�Y(unused)
                pfilename    : �t�@�C����(unused)

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

  Description:  �w���v�̕\��

  Arguments:    pfiletype    : �t�@�C���]�����[�h(unused)
                pfilesize    : �f�[�^�]���T�C�Y(unused)
                pfilename    : �t�@�C����(unused)

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

  Description:  �t�@�C���̃��X�g�\��

  Arguments:    pfiletype            : �t�@�C���]�����[�h
                pfilename            : LIST�R�}���h�̏ꍇ�ɂ̂ݎg�p����t�@�C����
                download_filedata    : �_�E�����[�h����t�@�C���̃f�[�^
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

    // ����result�ɂ̓f�[�^�R�l�N�V�����̃\�P�b�g���Ԃ��Ă��Ă���
    passive_result = FTP_Passive(filetype);
    if (passive_result < 0)
        OS_TPrintf("failed in FTP_Passive\n");

    // option��TRUE�̂Ƃ���LIST�R�}���h
    if (option)
    {
        send_result = FTP_OnlySendCommand("LIST %s\r\n", filename);
    }

    // option��FALSE�̂Ƃ���NLST -1�R�}���h
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
            // �f�[�^�͑S�ēǂݍ���
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

  Description:  �f�B���N�g�����̃t�@�C���������X�g�\���A�ۑ����܂��B

  Arguments:    tab                : ��������
                pe                 : ������̃f�B���N�g���AFSDirEntry�\���̂̃A�h���X
                output_main        : TURE�Ȃ炻�̃f�B���N�g���ȉ��̃f�B���N�g���A�t�@�C������\��
                                     FALSE�Ȃ炻�̃f�B���N�g���̃t�@�C������ۑ��B
                upload_filename    : �t�@�C�����̕ۑ���

  Returns:      �\���܂��͕ۑ������f�B���N�g���A�t�@�C����
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

                    // ��؂�̂��߉��s����������
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

  Description:  �A�b�v���[�h����t�@�C���̃f�[�^���擾

  Arguments:    buf    : �t�@�C���f�[�^�i�[��o�b�t�@�ւ̃|�C���^
                len    : �o�b�t�@�̃T�C�Y
                p_file : FSFile�\���̂̃A�h���X

  Returns:      �A�b�v���[�h����t�@�C���̃f�[�^�ANULL �Ȃ玸�s.
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
