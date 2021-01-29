/*---------------------------------------------------------------------------*
  Project:  NitroWiFi - demo - soc - ftpclient
  File:     main.c

  Copyright 2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: main.c,v $
  Revision 1.5  2006/03/10 09:22:43  kitase_hirotake
  INDENT SOURCE

  Revision 1.4  2006/01/16 03:46:50  okubata_ryoma
  small fix

  Revision 1.2  2006/01/12 02:22:55  okubata_ryoma
  small fix

  Revision 1.1  2006/01/12 02:18:18  okubata_ryoma
  ftpclient�̒ǉ�


  $NoKeywords: $
 *---------------------------------------------------------------------------*/
#include <nitro.h>
#include <nitroWiFi.h>
#include <stdlib.h>
#include <sitedefs.h>

#include "ftp.h"
#include "netconnect.h"

void    Test_FTPclient(void);
void    Test_SelectMode(BOOL filetype, char* filename, int filesize);

BOOL    UploadReady(char* filename);
BOOL    DownloadReady(BOOL filetype, int* filesize, char* filename);
int     RecursiveEnumDirEx(int tab, FSDirEntry* pe, BOOL output_main, char download_filename[FILEDATA256]);
void    CursorSelect(int Max, int Min, int* line);

#define SITEDEFS_FTPCLASS   "WiFi.LAN.1.FTP.1"  // �ڑ�����FTP�T�[�o(�ڑ��ł��Ȃ��ꍇ��SiteDefs/sitedefs.c��ύX���Ă�������)
#define FTP_COMMAND_ADDR    "192.168.105.201"   // �ڑ�����FTP�T�[�o�̃A�h���X
#define FTP_COMMAND_PORT    21                  // �ڑ�����FTP�T�[�o�̃|�[�g
#define FTP_FILE_SIZE       102400              // �f�[�^�]������t�@�C���T�C�Y
#define FTP_USER_NAME       "ftp"               // FTP�T�[�o�Ƀ��O�C�����邽�߂̃��[�UID
#define FTP_PASSWORD        "abc@foo.com"       // FTP�T�[�o�Ƀ��O�C�����邽�߂̃p�X���[�h
#define SITEDEFS_APCLASS    "WiFi.LAN.1.AP.1"   // �ڑ�����DHCP�T�[�o(�ڑ��ł��Ȃ��ꍇ��SiteDefs/sitedefs.c��ύX���Ă�������)

// key information
u16 Trg;
u16 Cont;
u16 keyData;

/*****************************************************************************/

/* function */
void NitroMain(void)
{
    const u32   default_dma_no = 2;             //MI_DMA_MAX_NUM;

    //---- DHCP�T�[�o�֐ڑ�
    NcGlobalInit();
    NcStart(SITEDEFS_APCLASS);

    /* �X�N���[���̏����� */
    InitScreen();
    FlushScreen();
    FlushDebugOutput();

    InitDebugOutputHook();

    /* �t�@�C���V�X�e���̏����� */
    FS_Init(default_dma_no);

    /* always preload FS table for faster directory access. */
    {
        u32     need_size = FS_GetTableSize();
        void*   p_table = OS_Alloc(need_size);
        SDK_ASSERT(p_table != NULL);
        (void)FS_LoadTable(p_table, need_size);
    }

    // �L�[���͏��擾�̋�Ăяo��(IPL �ł� A �{�^�������΍�)
    (void)PAD_Read();

    // FTPclient�e�X�g�J�n
    Test_FTPclient();

    NcFinish();
}

void Test_FTPclient()
{
    int     result;
    BOOL    flag_open;                          // FTP_Open�ɐ���������TRUE
    BOOL    flag_login;                         // FTP_Login�ɐ���������TRUE
    BOOL    isvalid = FALSE;
    char*   host_addr = FTP_COMMAND_ADDR;       // �ڑ�����FTP�T�[�o�̃A�h���X(default:192.168.105.201)
    u16     host_port = FTP_COMMAND_PORT;       // �ڑ�����FTP�T�[�o�̃|�[�g(default:21)
    const char*     user_name = FTP_USER_NAME;  // FTP�T�[�o�Ƀ��O�C�����邽�߂̃��[�UID
    const char*     password = FTP_PASSWORD;    // FTP�T�[�o�Ƀ��O�C�����邽�߂̃p�X���[�h
    BOOL        filetype = TRUE;                // �t�@�C���]�����[�h
    char        filename[32];                   // �f�[�^�]������t�@�C����

    // "/data/hello.txt", "/data/upload_test.txt", "/src/main.c" �Ȃ�
    int filesize = FTP_FILE_SIZE;               // �f�[�^�]������t�@�C���T�C�Y(default:102400byte)
    MI_CpuClear8(filename, sizeof(filename));

    ENV_SetClass(SITEDEFS_FTPCLASS);

    if (ENV_GetBOOL(".ISVALID", &isvalid) && isvalid && ENV_GetString(".NAME", &host_addr) &&
        ENV_GetU16(".PORT.1", &host_port))
    {
        while (1)
        {
            flag_open = FALSE;
            flag_login = FALSE;

            //---- read pad data
            keyData = PAD_Read();
            Trg = (u16) (keyData & (keyData ^ Cont));
            Cont = keyData;

            PrintString(2, 10, GetFlickerColor(), "Press A button to ftp open.");
            PrintString(3, 13, COLOR_WHITE, "Push UP       : Cursor UP");
            PrintString(3, 14, COLOR_WHITE, "Push DOWN     : CurSor Down");
            PrintString(3, 15, COLOR_WHITE, "Push A Button : Menu Select");
            PrintString(3, 16, COLOR_WHITE, "Push B Button : Cancel");

            // A�{�^����FTP�ڑ��J�n
            if (Trg == PAD_BUTTON_A)
            {
                // FTP �̃Z�b�V�������J��
                OS_TPrintf("ftp session for FTP server. \n");

                result = FTP_Open(host_addr, host_port);
                if (result < 0)
                    OS_TPrintf("failed in FTP_Open\n");
                else
                    flag_open = TRUE;

                result = FTP_Login(user_name, password);
                if (result < 0)
                    OS_TPrintf("failed in FTP_Login\n");
                else
                    flag_login = TRUE;

                ClearScreen();
            }

            // FTP sever �Ƀ��O�C�������̂ŁA�R�}���h�v����҂�
            if (flag_open && flag_login)
            {
                Test_SelectMode(filetype, filename, filesize);
            }
        }
    }
}

/*---------------------------------------------------------------------------*
  Name:         Test_SelectMode

  Description:  �R�}���h��I���ł���e�X�g���[�h

  Arguments:    filetype    : �t�@�C���]�����[�h
                filename    : �t�@�C����
                filesize    : �f�[�^�]���T�C�Y

  Returns:      None.
 *---------------------------------------------------------------------------*/
void Test_SelectMode(BOOL filetype, char* filename, int filesize)
{
    int     line = UPLOAD;      // �J�[�\���̏����ʒu
    BOOL    end_flag = FALSE;   // �e�X�g�I���t���O
    BOOL    cancel_flag;        // �v���_�E�����j���[�̃L�����Z���t���O
    while (1)
    {
        //---- read pad data
        keyData = PAD_Read();
        Trg = (u16) (keyData & (keyData ^ Cont));
        Cont = keyData;

        PrintString(3, UPLOAD, COLOR_WHITE, "File Upload");
        PrintString(3, DOWNLOAD, COLOR_WHITE, "File Download");
        PrintString(3, NLST, COLOR_WHITE, "Send NLST");
        PrintString(3, QUIT, COLOR_WHITE, "Send QUIT");
        PrintString(3, ASCII, COLOR_WHITE, "ASCII Mode");
        PrintString(3, BINARY, COLOR_WHITE, "BINARY Mode");
        PrintString(3, LS, COLOR_WHITE, "ls");
        PrintString(3, HELP, COLOR_WHITE, "Help");
        PrintString(2, line, GetFlickerColor(), "@");

        // �㉺�L�[�Ń��j���[�I��
        CursorSelect(UPLOAD, HELP, &line);

        if (Trg == PAD_BUTTON_A)
        {
            switch (line)
            {
            case UPLOAD:
                ClearScreen();
                cancel_flag = UploadReady(filename);
                if (!cancel_flag)
                    FTP_Upload((void*)filetype, (void*)filesize, (void*)filename);
                break;

            case DOWNLOAD:
                ClearScreen();
                cancel_flag = DownloadReady(filetype, &filesize, filename);
                if (!cancel_flag)
                    FTP_Download((void*)filetype, (void*)filesize, (void*)filename);
                break;

            case NLST:
                FTP_Nlst((void*)filetype, NULL, NULL);
                break;

            case QUIT:
                FTP_Logout(NULL, NULL, NULL);
                end_flag = TRUE;
                break;

            case ASCII:
                FTP_ASCMode((void*)filetype, NULL, NULL);
                break;

            case BINARY:
                FTP_BINMode((void*)filetype, NULL, NULL);
                break;

            case LS:
                FTP_Ls(NULL, NULL, NULL);
                break;

            case HELP:
                FTP_Help(NULL, NULL, NULL);
                break;

            default:
                break;
            }

            if (end_flag)
                break;
        }
    }
}

/*---------------------------------------------------------------------------*
  Name:         UploadReady

  Description:  �A�b�v���[�h�ł���t�@�C�������X�g��\�����A�I�����ꂽ�t�@�C�������擾

  Arguments:    filename    : �t�@�C����

  Returns:      cancel_flag�̒l
 *---------------------------------------------------------------------------*/
BOOL UploadReady(char* filename)
{
    FSFile      d;
    FSDirEntry  e;

    int         line = UPLOAD;
    BOOL        cancel_flag = FALSE;
    char        upload_filename[FILEDATA256];   // �A�b�v���[�h����t�@�C����
    while (1)
    {
        int     line_plus = 0;
        char*   return_point;
        char*   pupload_filename;
        int     filename_len = 0;
        char    print_filename[FILEDATA256];
        int     i;

        FS_InitFile(&d);
        (void)FS_FindDir(&d, "/");

        //---- read pad data
        keyData = PAD_Read();
        Trg = (u16) (keyData & (keyData ^ Cont));
        Cont = keyData;

        MI_CpuClear8(upload_filename, sizeof(upload_filename));

        // �A�b�v���[�h�ł���t�@�C�������X�g�̕\��
        PrintString(3, UPLOAD, COLOR_WHITE, "File Upload");
        PrintString(2, line + 1, GetFlickerColor(), "@");
        MI_WriteByte(e.name + 0, (u8) '\0');
        (void)FS_TellDir(&d, &e.dir_id);
        line_plus = RecursiveEnumDirEx(2, &e, FALSE, upload_filename);

        // �㉺�L�[�Ń��j���[�I��
        CursorSelect(UPLOAD, line_plus + (UPLOAD - 1), &line);

        // A�{�^���ŃA�b�v���[�h����t�@�C��������
        if (Trg == PAD_BUTTON_A)
        {
            MI_CpuClear8(print_filename, sizeof(print_filename));

            ClearScreen();

            // �A�b�v���[�h����t�@�C�����̎擾
            pupload_filename = upload_filename;
            for (i = 0; i <= line - UPLOAD; i++)
            {
                return_point = STD_StrStr(pupload_filename, "\n");
                filename_len = (int)(return_point - pupload_filename);
                (void)STD_StrLCpy(print_filename, pupload_filename, filename_len);

                pupload_filename += filename_len + 1;
            }

            MI_CpuCopy8(print_filename, filename, sizeof(print_filename));
            break;
        }

        // B�{�^���Ń��C�����j���[�ɖ߂�
        else if (Trg == PAD_BUTTON_B)
        {
            ClearScreen();
            cancel_flag = TRUE;
            break;
        }
    }

    return cancel_flag;
}

/*---------------------------------------------------------------------------*
  Name:         DownloadReady

  Description:  �_�E�����[�h�ł���t�@�C�������X�g��\�����A�I�����ꂽ�t�@�C�������擾

  Arguments:    filetype    : �t�@�C���]�����[�h
                filesize    : �f�[�^�]���T�C�Y
                filename    : �t�@�C����

  Returns:      cancel_flag�̒l
 *---------------------------------------------------------------------------*/
BOOL DownloadReady(BOOL filetype, int* filesize, char* filename)
{
    int     line = DOWNLOAD;
    BOOL    cancel_flag = FALSE;

    char    download_filename[FILEDATA256]; // �_�E�����[�h����t�@�C����
    char    download_filedata[FILEDATA256]; // �_�E�����[�h����t�@�C���T�C�Y
    MI_CpuClear8(download_filename, sizeof(download_filename));

    FTP_LIST((void*)filetype, (void*)filename, download_filename, FALSE);
    while (1)
    {
        int     line_plus = 0;
        char*   return_point;
        char*   pdownload_filename;         // �_�E�����[�h����t�@�C�����ւ̃|�C���^
        char*   pdownload_filedata;         // �_�E�����[�h����t�@�C���T�C�Y�ւ̃|�C���^
        int     filename_len = 0;           // �t�@�C�����̕�����Ƃ��Ă̒���
        int     filesize_len = 0;           // �t�@�C���T�C�Y�̕�����Ƃ��Ă̒���
        char    print_filename[FILEDATA256];
        char    size[FILEDATA16];           // �t�@�C���T�C�Y
        int     i;

        //---- read pad data
        keyData = PAD_Read();
        Trg = (u16) (keyData & (keyData ^ Cont));
        Cont = keyData;

        // �_�E�����[�h�ł���t�@�C�������X�g�̕\��
        PrintString(3, DOWNLOAD, COLOR_WHITE, "File Download");
        PrintString(2, line + 1, GetFlickerColor(), "@");
        pdownload_filename = download_filename;
        while (*pdownload_filename)
        {
            return_point = STD_StrStr(pdownload_filename, "\n");
            filename_len = (int)(return_point - pdownload_filename);
            (void)STD_StrLCpy(print_filename, pdownload_filename, filename_len);

            line_plus++;
            PrintString(4, DOWNLOAD + line_plus, COLOR_WHITE, print_filename);
            pdownload_filename += filename_len + 1;
        }

        // �㉺�L�[�Ń��j���[�I��
        CursorSelect(DOWNLOAD, line_plus + (DOWNLOAD - 1), &line);

        // A�{�^���Ń_�E�����[�h����t�@�C����������
        if (Trg == PAD_BUTTON_A)
        {
            MI_CpuClear8(print_filename, sizeof(print_filename));
            MI_CpuClear8(download_filedata, sizeof(download_filedata)); //
            ClearScreen();

            // �_�E�����[�h����t�@�C�����̎擾
            pdownload_filename = download_filename;
            for (i = 0; i <= line - DOWNLOAD; i++)
            {
                return_point = STD_StrStr(pdownload_filename, "\n");
                filename_len = (int)(return_point - pdownload_filename);
                (void)STD_StrLCpy(print_filename, pdownload_filename, filename_len);

                pdownload_filename += filename_len + 1;
            }

            MI_CpuCopy8(print_filename, filename, sizeof(print_filename));

            // �_�E�����[�h����t�@�C���̃T�C�Y����
            FTP_LIST((void*)filetype, (void*)filename, download_filedata, TRUE);

            pdownload_filedata = download_filedata;

            // �O����S�̃X�y�[�X������łT���ڂ��t�@�C���T�C�Y�Ȃ̂�
            for (i = 0; i < 4; i++)
            {
                return_point = STD_StrStr(pdownload_filedata, " ");
                filesize_len = (int)(return_point - pdownload_filedata);
                pdownload_filedata += filesize_len + 1;

                if (STD_StrStr(pdownload_filedata, " "))
                {
                    while (!STD_StrNCmp(pdownload_filedata, " ", 1))
                    {
                        pdownload_filedata++;
                    }
                }

            }

            MI_CpuClear8(size, sizeof(size));
            return_point = STD_StrStr(pdownload_filedata, " ");
            filename_len = (int)(return_point - pdownload_filedata);
            MI_CpuCopy8(pdownload_filedata, size, (u32) filesize_len);
            *filesize = atoi(size);
            break;
        }

        // B�{�^���Ń��C�����j���[�ɖ߂�
        else if (Trg == PAD_BUTTON_B)
        {
            ClearScreen();
            cancel_flag = TRUE;
            break;
        }
    }

    return cancel_flag;
}

/*---------------------------------------------------------------------------*
  Name:         CursorSelect

  Description:  �J�[�\���ړ��̐���

  Arguments:    Max    : �J�[�\���̍������x
                Min    : �J�[�\���̒Ⴓ���x
                line   : �J�[�\���̌��ݒn
                
  Returns:      None.
 *---------------------------------------------------------------------------*/
void CursorSelect(int Max, int Min, int* line)
{
    if (Trg == PAD_KEY_UP)
    {
        if (*line == Max)
        {
            *line = Min;
        }
        else
        {
            (*line)--;
        }

        ClearScreen();
    }
    else if (Trg == PAD_KEY_DOWN)
    {
        if (*line == Min)
        {
            *line = Max;
        }
        else
        {
            (*line)++;
        }

        ClearScreen();
    }
}

/*---------------------------------------------------------------------------*/

/* END OF FILE */
