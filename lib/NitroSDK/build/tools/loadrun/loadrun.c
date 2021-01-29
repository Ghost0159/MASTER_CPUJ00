/*---------------------------------------------------------------------------*
  Project:  NitroSDK - tools - buryarg
  File:     buryarg.c

  Copyright 2005,2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: loadrun.c,v $
  Revision 1.15  2006/11/28 04:29:28  yada
  modify help message

  Revision 1.14  2006/01/18 02:11:21  kitase_hirotake
  do-indent

  Revision 1.13  2005/09/05 10:54:13  yada
  --type (device type) option was deleted

  Revision 1.12  2005/09/05 01:34:33  yasu
  version.h ���Q�Ƃ��\�[�X�t�@�C���̍ŋ߂̃^�C���X�^���v��\������悤�ɂ���

  Revision 1.11  2005/09/01 07:21:24  yada
  change exit status

  Revision 1.10  2005/08/31 12:14:27  yada
  applied for OS_Exit()

  Revision 1.9  2005/08/30 01:47:33  yada
  become to be able to load from stdin

  Revision 1.8  2005/08/29 10:00:01  seiki_masashi
  �W���o�͂̃o�b�t�@�����O�𖳌���

  Revision 1.7  2005/08/25 02:16:11  yada
  able to use Abort string at QuietMode

  Revision 1.6  2005/08/25 00:35:46  yada
  lap time

  Revision 1.5  2005/08/24 13:02:03  yada
  applied for exec timeout

  Revision 1.4  2005/08/24 12:56:53  yada
  change ctrl-c proc

  Revision 1.3  2005/08/24 11:23:00  yada
  lock option of card and cartridge slot are OFF as default

  Revision 1.2  2005/08/24 09:53:35  yada
  applied for new ISD library

  Revision 1.1  2005/08/24 07:44:46  yada
  (none)

  $NoKeywords: $
 *---------------------------------------------------------------------------*/
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/fcntl.h>
#include <sys/stat.h>
#include <getopt.h>
#include <signal.h>
#include <time.h>
#include <version.h>

#include "ISNITRODLL.h"
#include "isd_api.h"

#define  SDK_BOOL_ALREADY_DEFINED_
#include <nitro_win32.h>
#include <nitro/os/common/system.h>

//---- �o�[�W����������
#define VERSION_STRING     " 1.12  Copyright 2005,2006 Nintendo. All right reserved."
// 1.12 �w���v�\�����C��
// 1.11 �f�o�C�X�^�C�v�폜
// 1.10 �\�[�X�t�@�C���̍ŋ߂̃^�C���X�^���v�\��
// 1.9 loadrun���g��exit status ��ύX����
// 1.8 OS_Exit() ����̏I��������ɑΉ�����
// 1.7 �W�����͂���ǂ߂�悤�ɂ���
// 1.6 �W���o�͂̃o�b�t�@�����O�𖳌�������
// 1.5 QuietMode �ł�������I�����\�ɂ���
// 1.4 ���b�v�\������
// 1.3 ���s�^�C���A�E�g����
// 1.2 ctrl-c �Œ�~�����Ƃ��̏�����ύX
// 1.1 ctrl-c �Œ�~�����Ƃ��ɃX���b�g���I�t�ɂ���悤�ɂ���
// 1.0 ���J

//---- exit �ԍ�
#define EXIT_NUM_NO_ERROR               207     // �����I��(�A���A����͂���ɂȂ邱�Ƃ͂Ȃ�)
#define EXIT_NUM_USER_SIGNAL            206     // ���[�U�ɂ�鋭���I��(ctrl-C)
#define EXIT_NUM_EXEC_TIME_OUT          205     // ���s�^�C���A�E�g�ŋ����I��
#define EXIT_NUM_TIME_OUT               204     // �\���^�C���A�E�g�ŋ����I��
#define EXIT_NUM_SHOW_DEVICES           203     // �f�o�C�X���X�g�\���ŏI��
#define EXIT_NUM_SHOW_USAGE             202     // �w���v�\���ŏI��
#define EXIT_NUM_SHOW_VERSION           201     // �o�[�W�����\���ŏI��
#define EXIT_NUM_STRING_ABORT           200     // ������ɂ�鋭���I��

#define EXIT_NUM_NO_DEVICE              -1      // �g�p�\�ȃf�o�C�X���Ȃ�
#define EXIT_NUM_UNKNOWN_OPTION         -2      // �z��O�̃I�v�V�������w�肳�ꂽ
#define EXIT_NUM_ILLEGAL_OPTION         -3      // �I�v�V�����̎g�p���@���Ⴄ
#define EXIT_NUM_NO_INPUT_FILE          -4      // �w��̃t�@�C�����Ȃ�or�I�[�v���ł��Ȃ�
#define EXIT_NUM_NOT_CONNECT            -5      // �f�o�C�X�ւ̐ڑ����s
#define EXIT_NUM_CANNOT_USE_CARTRIDGE   -6      // �J�[�g���b�W�̃��b�N�Ɏ��s
#define EXIT_NUM_CANNOT_USE_CARD        -7      // �J�[�h�̃��b�N�Ɏ��s
#define EXIT_NUM_PRINTF_ERROR           -8      // printf�f�[�^�������̃G���[
#define EXIT_NUM_LOADING_ERROR          -9      // ���[�h���̃G���[

//---- �f�o�C�X�w��p
char   *gDeviceName[] = {
    "CGBUSB", "CGBSCSI", "NITROUSB", "NITROUIC", NULL
};
int     gDeviceTypeArray[] = {
    ISNTD_DEVICE_CGB_EMULATOR_USB,
    ISNTD_DEVICE_CGB_EMULATOR_SCSI,
    ISNTD_DEVICE_IS_NITRO_EMULATOR,
    ISNTD_DEVICE_IS_NITRO_UIC
};

//---- ���샂�[�h
BOOL    gQuietMode = FALSE;            // quiet ���[�h
BOOL    gVerboseMode = FALSE;          // verbose ���[�h
BOOL    gDebugMode = FALSE;            // debug ���[�h

BOOL    gStdinMode = FALSE;            // stdin ���[�h

BOOL    gIsTypeSpecified = FALSE;      // �f�o�C�X�^�C�v�w�肠�肩�H
int     gSpecifiedType;                // ����ꍇ�̃f�o�C�X�^�C�v

BOOL    gIsSerialSpecified = FALSE;    // �V���A���ԍ��w�肠�肩�H
int     gSpecifiedSerial;              // ����ꍇ�̃V���A���ԍ�

BOOL    gIsCartridgeLocked = FALSE;    // �J�[�g���b�W�X���b�g�����b�N���邩
BOOL    gIsCardLocked = FALSE;         // �J�[�h�X���b�g�����b�N���邩

int     gTimeOutTime = 0;              // �^�C���A�E�g����(�b) (0 �͂Ȃ�)
BOOL    gTimeOutOccured = FALSE;       // �^�C���A�E�g������������

int     gExecTimeOutTime = 0;          // ���s�^�C���A�E�g����(�b) (0 �͂Ȃ�)
int     gExecTimeOutOccured = FALSE;   // ���s�^�C���A�E�g������������

char   *gAbortString = NULL;           // �����I��������
BOOL    gStringAborted = FALSE;        // �����I��������ɂ���ďI��������ǂ���

BOOL    gExitAborted = FALSE;          // OS_Exit() �I��
int     gExitStatusNum = EXIT_NUM_STRING_ABORT; // OS_Exit()�ɂ���ďI������Ƃ��̕Ԃ�l
int     gExitStrLength;                // �I��������̃T�C�Y

//---- NITRO���C�u�����p
HINSTANCE gDllInstance;
NITRODEVICEHANDLE gDeviceHandle;
NITRODEVICEID gDeviceId;

//---- �f�o�C�X�ɐڑ�������(�X���b�g�̃X�C�b�`�p)
BOOL    gDeviceConnected = FALSE;


//---- �f�o�C�X���X�g
#define DEVICE_MAX_NUM      256
#define DEVICE_SERIAL_NONE  0x7fffffff // means no specified
ISNTDDevice gDeviceList[DEVICE_MAX_NUM];
int     gCurrentDevice = -1;
int     gConnectedDeviceNum = 0;

int     gDeviceTypeSpecified = ISNTD_DEVICE_NONE;
int     gDeviceSerialSpecified = DEVICE_SERIAL_NONE;    // means no specified

//---- ���̓t�@�C��
#define FILE_NAME_MAX_SIZE  1024
FILE   *gInputFile;
char    gInputFileNameString[FILE_NAME_MAX_SIZE];
BOOL    gIsInputFileOpened = FALSE;

//---- ����
time_t  gStartTime = 0;                // �X�^�[�g����
BOOL    gIsLineHead = TRUE;            // �s�����ǂ���
BOOL    gShowLapTime = FALSE;

//---- �V�O�i��
BOOL    gStoppedByUser = FALSE;        // ���[�U�Ɏ~�߂�ꂽ���ǂ���


#define printfIfNotQuiet(...)    do{if(!gQuietMode){printf(__VA_ARGS__);}}while(0)

void    displayErrorAndExit(int exitNum, char *message);
BOOL    outputString(char *buf, int bufSize);

/*---------------------------------------------------------------------------*
  Name:         myExit

  Description:  similar to exit()

  Arguments:    exitNum : exit()�ԍ�

  Returns:      None
 *---------------------------------------------------------------------------*/
void myExit(int exitNum)
{
    //---- �J�[�g���b�W�ƃJ�[�h�̃X���b�g���I�t��
    if (gDeviceConnected)
    {
        (void)ISNTD_CartridgeSlotPower(gDeviceHandle, FALSE);
        (void)ISNTD_CardSlotPower(gDeviceHandle, FALSE);
    }

    //---- DLL�̈���
    ISNTD_FreeDll();

    if (!gQuietMode)
    {
        if (exitNum == EXIT_NUM_USER_SIGNAL)
        {
            printf("\n*** loadrun: stopped by user.\n");
        }
        else if (exitNum == EXIT_NUM_TIME_OUT)
        {
            printf("\n*** loadrun: stopped by print timeout.\n");
        }
        else if (exitNum == EXIT_NUM_EXEC_TIME_OUT)
        {
            printf("\n*** loadrun: stopped by exec timeout.\n");
        }
        else if (gStringAborted)
        {
            printf("\n*** loadrun: stopped by aborting string.\n");
        }
        else if (gExitAborted)
        {
            exitNum = gExitStatusNum;
        }
    }

    exit(exitNum);
}

/*---------------------------------------------------------------------------*
  Name:         listDevice

  Description:  �f�o�C�X�̈ꗗ�\�����ďI���

  Arguments:    None

  Returns:      None
 *---------------------------------------------------------------------------*/
void listDevice(void)
{
    int     n;

    //---- �f�o�C�X�ǂݍ���
    gConnectedDeviceNum = ISNTD_GetDeviceList(&gDeviceList[0], DEVICE_MAX_NUM);
    if (gConnectedDeviceNum < 0)
    {
        displayErrorAndExit(EXIT_NUM_NO_DEVICE, "Cannot access devices.");
    }

    printf("---- Connected devices:\n");

    for (n = 0; n < gConnectedDeviceNum; n++)
    {
        switch (gDeviceList[n].type)
        {
        case ISNTD_DEVICE_CGB_EMULATOR_USB:
            printf("%3d: [CGBUSB]  IS-CGB-EMU(USB) serial:%8d\n", n, gDeviceList[n].serial);
            break;
        case ISNTD_DEVICE_CGB_EMULATOR_SCSI:
            printf("%3d: [CGBSCSI]  IS-CGB-EMULATOR serial(host-id):%02d%02d\n", n,
                   gDeviceList[n].host, gDeviceList[n].serial);
            break;
        case ISNTD_DEVICE_IS_NITRO_EMULATOR:
            printf("%3d: [NITROUSB]  IS-NITRO-EMULATOR serial:%08d\n", n, gDeviceList[n].serial);
            break;
        case ISNTD_DEVICE_IS_NITRO_UIC:
            printf("%3d: [NITROUIC]  IS-NITRO-UIC serial: %08d\n", n, gDeviceList[n].serial);
            break;
        case ISNTD_DEVICE_UNKNOWN:
            printf("%3d: unknown device %x\n", n, (int)gDeviceList[n].ntdId);
            break;
        default:
            printf("Illegal device\n");
            break;
        }
    }

    //---- ��������
    printf("%d device(s) found.\n", gConnectedDeviceNum);

    myExit(EXIT_NUM_SHOW_DEVICES);
}

/*---------------------------------------------------------------------------*
  Name:         searchDevice

  Description:  search device

  Arguments:    None

  Returns:      None
 *---------------------------------------------------------------------------*/
void searchDevice(void)
{
    //---- �f�o�C�X�����������ꍇ
    if (gConnectedDeviceNum <= 0)
    {
        displayErrorAndExit(EXIT_NUM_NO_DEVICE, "found no device.");
    }

    //---- �Ȃɂ��̎w�肪�����
    if (gDeviceTypeSpecified != ISNTD_DEVICE_NONE || gDeviceSerialSpecified != DEVICE_SERIAL_NONE)
    {
        int     n;
        gCurrentDevice = -1;
        for (n = 0; n < gConnectedDeviceNum; n++)
        {
            //---- �w��f�o�C�X�Ƃ̃}�b�`����
            if (gDeviceTypeSpecified != ISNTD_DEVICE_NONE
                && gDeviceTypeSpecified != gDeviceList[n].type)
            {
                continue;
            }

            //---- �w��V���A���Ƃ̃}�b�`����
            if (gDeviceSerialSpecified != DEVICE_SERIAL_NONE
                && gDeviceSerialSpecified != gDeviceList[n].serial)
            {
                continue;
            }

            gCurrentDevice = n;
            break;
        }
    }
    //---- �w�肪�Ȃ��Ȃ�ŏ��̂���
    else
    {
        gCurrentDevice = 0;
    }

    //---- �w�肵�����̂��Ȃ��A�܂��͊Ԉ���Ă���
    if (gCurrentDevice < 0
        || gDeviceList[gCurrentDevice].type == ISNTD_DEVICE_NONE
        || gDeviceList[gCurrentDevice].type == ISNTD_DEVICE_UNKNOWN)
    {
        displayErrorAndExit(EXIT_NUM_NO_DEVICE, "illegal device.");
    }
}

/*---------------------------------------------------------------------------*
  Name:         displayUsage

  Description:  �g������\������

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void displayUsage(void)
{
    fprintf(stderr,
            "NITRO-SDK Development Tool - loadrun - Execute NITRO ROM image\n"
            "Build %lu\n\n"
            "Usage: loadrun [OPTION] <SrlFile>\n"
            "\t\tdownload Nitro srl file to debugger and execute.\n\n"
            "Options:\n"
            "  --version                   : Show version.\n"
            "  -h, --help                  : Show this help.\n"
            "  -q, --quiet                 : Quiet mode.\n"
            "  -v, --verbose               : Verbose mode.\n"
            "  -L, --list                  : List connecting device.\n"
            "  -l, --lap                   : Show lap time at each line.\n"
//            "  -d, --type=DEVICE           : Specify device type.\n"
//            "                                DEVICE=CGBUSB|CGBSCSI|NITROUSB|NITROUIC.\n"
            "  -s, --serial=SERIAL         : Specify serial number.\n"
            "  -t, --timeout=SECOND        : Specify quit time after last print.\n"
            "  -T, --exec-timeout=SECOND   : Specify quit time after execute program.\n"
            "  -a, --abort-string=STRING   : Specify aborting string.\n"
            "  -c, --card-slot=SWITCH      : Card      slot SWITCH=ON|OFF, default OFF.\n"
            "  -C, --cartridge-slot=SWITCH : Cartridge slot SWITCH=ON|OFF, default OFF.\n"
            "  --stdin, --standard-input   : Read data from stdin instead of <SrlFile>.\n\n",
            SDK_DATE_OF_LATEST_FILE);
}

/*---------------------------------------------------------------------------*
  Name:         displayVersion

  Description:  �o�[�W�����\������

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void displayVersion(void)
{
    printf("*** loadrun: %s\n", VERSION_STRING);
}

/*---------------------------------------------------------------------------*
  Name:         displayErrorAndExit

  Description:  �G���[��\������

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void displayErrorAndExit(int exitNum, char *message)
{
    printf("*** loadrun: Error: %s\n", message);

    //---- DLL�̈���
    ISNTD_FreeDll();

    exit(exitNum);
}

/*---------------------------------------------------------------------------*
  Name:         parseOption

  Description:  parse option line

  Arguments:    argc : argument count
                argv : argument vector

  Returns:      result. less than 0 if error.
 *---------------------------------------------------------------------------*/
void parseOption(int argc, char *argv[])
{
    int     n;
    int     c;
    BOOL    helpFlag = FALSE;

    struct option optionInfo[] = {
        {"help", no_argument, NULL, 'h'},
        {"quiet", no_argument, NULL, 'q'},
        {"verbose", no_argument, NULL, 'v'},
        {"list", no_argument, NULL, 'L'},
        {"lap", no_argument, NULL, 'l'},
        {"debug", no_argument, NULL, 'D'},      //�B���I�v�V����
        {"version", no_argument, NULL, '1'},
        {"stdin", no_argument, NULL, 'I'},
        {"standard-input", no_argument, NULL, 'I'},
        {"type", required_argument, 0, 'd'},    //�B���I�v�V����
        {"serial", required_argument, 0, 's'},
        {"timeout", required_argument, 0, 't'},
        {"exec-timeout", required_argument, 0, 'T'},
        {"abort-string", required_argument, 0, 'a'},
        {"card-slot", required_argument, 0, 'c'},
        {"cartridge-slot", required_argument, 0, 'C'},
        {NULL, 0, 0, 0}
    };
    int     optionIndex;

    char   *optionStr = NULL;

    //---- suppress error string of getopt_long()
    opterr = 0;

    while (1)
    {
        c = getopt_long(argc, argv, "+hqvlLDd:s:t:T:a:c:C:", &optionInfo[0], &optionIndex);

        //printf("optind=%d optopt=%d  %x(%c) \n", optind, optopt, c,c );

        if (c == -1)
        {
            break;
        }

        switch (c)
        {
        case 'I':                     //---- �W������
            gStdinMode = TRUE;
            break;
        case 'h':                     //---- �w���v�\��
            helpFlag = TRUE;
            break;
        case 'q':                     //---- quiet���[�h
            gQuietMode = TRUE;
            break;
        case 'v':                     //---- verbose���[�h
            gVerboseMode = TRUE;
            break;
        case 'D':                     //---- debug���[�h
            gDebugMode = TRUE;
            break;
        case '1':                     //---- �o�[�W�����\��
            displayVersion();
            myExit(EXIT_NUM_SHOW_VERSION);
            break;
        case 'L':                     //---- �f�o�C�X�̈ꗗ
            listDevice();
            break;
        case 'l':                     //---- ���b�v�^�C��
            gShowLapTime = TRUE;
            break;
        case 'd':                     //---- �f�o�C�X
            optionStr = (char *)(optarg + ((*optarg == '=') ? 1 : 0));
            {
                int     n;
                for (n = 0; gDeviceName[n]; n++)
                {
                    if (!strcmp(optionStr, gDeviceName[n]))
                    {
                        gDeviceTypeSpecified = gDeviceTypeArray[n];
                        break;
                    }
                }

                if (gDeviceTypeSpecified == ISNTD_DEVICE_NONE)
                {
                    displayErrorAndExit(EXIT_NUM_ILLEGAL_OPTION, "illegal device type.");
                }
            }
            break;
        case 's':                     //---- �V���A���w��
            optionStr = (char *)(optarg + ((*optarg == '=') ? 1 : 0));
            gDeviceSerialSpecified = atoi(optionStr);
            break;
        case 'c':                     //---- �J�[�h�X���b�g���b�N
            optionStr = (char *)(optarg + ((*optarg == '=') ? 1 : 0));
            if (!strcmp(optionStr, "ON") || !strcmp(optionStr, "on"))
            {
                gIsCardLocked = TRUE;
            }
            else if (!strcmp(optionStr, "OFF") || !strcmp(optionStr, "off"))
            {
                gIsCardLocked = FALSE;
            }
            else
            {
                displayErrorAndExit(EXIT_NUM_ILLEGAL_OPTION, "illegal value for card slot option.");
            }
            break;
        case 'C':                     //---- �J�[�g���b�W�X���b�g���b�N
            optionStr = (char *)(optarg + ((*optarg == '=') ? 1 : 0));
            if (!strcmp(optionStr, "ON") || !strcmp(optionStr, "on"))
            {
                gIsCartridgeLocked = TRUE;
            }
            else if (!strcmp(optionStr, "OFF") || !strcmp(optionStr, "off"))
            {
                gIsCartridgeLocked = FALSE;
            }
            else
            {
                displayErrorAndExit(EXIT_NUM_ILLEGAL_OPTION,
                                    "illegal value for cartridge slot option.");
            }
            break;
        case 't':                     //---- �ŏI�\������̃^�C���A�E�g����
            optionStr = (char *)(optarg + ((*optarg == '=') ? 1 : 0));
            gTimeOutTime = atoi(optionStr);
            if (gTimeOutTime <= 0)
            {
                displayErrorAndExit(EXIT_NUM_ILLEGAL_OPTION,
                                    "illegal value for abort timeout option.");
            }
            break;
        case 'T':                     //---- ���s�^�C���A�E�g����
            optionStr = (char *)(optarg + ((*optarg == '=') ? 1 : 0));
            gExecTimeOutTime = atoi(optionStr);
            if (gExecTimeOutTime <= 0)
            {
                displayErrorAndExit(EXIT_NUM_ILLEGAL_OPTION,
                                    "illegal value for abort exec timeout option.");
            }
            break;
        case 'a':                     //---- �I��������
            gAbortString = (char *)(optarg + ((*optarg == '=') ? 1 : 0));
            {
                int     length = strlen(gAbortString);
                if (length <= 0 || length > 256)
                {
                    displayErrorAndExit(EXIT_NUM_ILLEGAL_OPTION,
                                        "illegal value for abort string option.");
                }
            }
            break;
        default:
            displayErrorAndExit(EXIT_NUM_UNKNOWN_OPTION, "unknown option.");
        }
    }

    //---- �w���v�\��
    {
        BOOL    isDisplayHelp = FALSE;

        if (helpFlag)
        {
            isDisplayHelp = TRUE;
        }
        else if (argc <= optind && !gStdinMode)
        {
            isDisplayHelp = TRUE;
        }
        else if (argc > optind && gStdinMode)
        {
            isDisplayHelp = TRUE;
        }

        if (isDisplayHelp)
        {
            displayUsage();
            exit(EXIT_NUM_SHOW_USAGE);
        }
    }

    //---- ���̓t�@�C����
    if (!gStdinMode)
    {
        strncpy(gInputFileNameString, argv[optind], FILE_NAME_MAX_SIZE);
    }

    if (gVerboseMode)
    {
        if (!gStdinMode)
        {
            printf("Input file is [%s]\n", gInputFileNameString);
        }
        else
        {
            printf("Input file is stdin\n");
        }
    }

    //---- ���\��
    if (gVerboseMode)
    {
        printf("Print time out : %d sec.\n", gTimeOutTime);
        printf("Execute time out : %d sec.\n", gExecTimeOutTime);
        printf("Card lock : %s.\n", (gIsCardLocked) ? "ON" : "OFF");
        printf("Cartridge lock : %s.\n", (gIsCartridgeLocked) ? "ON" : "OFF");

        if (gAbortString)
        {
            printf("Abort string : [%s]\n", gAbortString);
        }
    }
}

/*---------------------------------------------------------------------------*
  Name:         loadFile

  Description:  load file

  Arguments:    None

  Returns:      None
 *---------------------------------------------------------------------------*/
void loadFile(void)
{
    unsigned int address = 0;
    //int fileSize;

    //---- �t�@�C���I�[�v��
    if (gStdinMode)
    {
        gInputFile = stdin;
        _setmode(_fileno(gInputFile), O_BINARY);
    }
    else
    {
        if ((gInputFile = fopen(gInputFileNameString, "rb")) == NULL)
        {
            displayErrorAndExit(EXIT_NUM_NO_INPUT_FILE, "cannot open input file.");
        }
    }
    gIsInputFileOpened = TRUE;

    //---- �f�o�C�X�ւ̐ڑ�
    if ((gDeviceHandle = ISNTD_DeviceOpen(gDeviceList[gCurrentDevice].ntdId)) == NULL)
    {
        displayErrorAndExit(EXIT_NUM_NOT_CONNECT, "cannot connect device.");
    }
    gDeviceConnected = TRUE;

    //---- ���Z�b�g�̔��s
    ISNTD_Reset(gDeviceHandle, TRUE);
    Sleep(1000);

    //---- �t�@�C���T�C�Y�擾
    //fseek( gInputFile, 0L, SEEK_END );
    //fileSize = ftell( gInputFile );
    //fseek( gInputFile, 0L, SEEK_SET );

    //---- 16KB���]������
    while (1)
    {
        char    buf[16384];
        size_t  size = fread(buf, 1, sizeof(buf), gInputFile);
        static int progressCount = 0;

        if (!size)
        {
            break;
        }

        //---- �]��
        if (!ISNTD_WriteROM(gDeviceHandle, buf, address, size))
        {
            displayErrorAndExit(EXIT_NUM_LOADING_ERROR, "troubled while loading input file.");
        }

        address += size;

        if (gVerboseMode)
        {
            if (!(progressCount++ % 32))
            {
                printf("*");
            }
        }
    }

    //---- �t�@�C���N���[�Y
    if (gStdinMode)
    {
        _setmode(_fileno(gInputFile), O_TEXT);
    }
    else
    {
        fclose(gInputFile);
    }
    gIsInputFileOpened = FALSE;

    if (gVerboseMode)
    {
        printf("\nInput file size: %d (0x%x) byte\n", address, address);
    }
}

/*---------------------------------------------------------------------------*
  Name:         setSlopPower

  Description:  �X���b�g�̃p���[����

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void setSlotPower(void)
{
    //---- �J�[�g���b�W�X���b�g�����b�N����
    if (gIsCartridgeLocked)
    {
        if (!ISNTD_CartridgeSlotPower(gDeviceHandle, TRUE))
        {
            displayErrorAndExit(EXIT_NUM_CANNOT_USE_CARTRIDGE, "cannot use cartridge slot.");
        }
    }

    //---- �J�[�h�X���b�g�����b�N����
    if (gIsCardLocked)
    {
        if (!ISNTD_CardSlotPower(gDeviceHandle, TRUE))
        {
            displayErrorAndExit(EXIT_NUM_CANNOT_USE_CARD, "cannot use card slot.");
        }
    }

    //---- ���Z�b�g�̉���
    Sleep(1000);
    ISNTD_Reset(gDeviceHandle, FALSE);
}

/*---------------------------------------------------------------------------*
  Name:         procPrint

  Description:  printf����

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
NITROArch archNum[] = {
    NITROArchARM9,
    NITROArchARM7
};

#define PRINT_ONETIME_SIZE  512
//#define PRINT_ONETIME_SIZE    16

//---- �������ĕ\�����邽�߂̃o�b�t�@
static char gConbineBuf[PRINT_ONETIME_SIZE * 2 + 2] = "\0";
static char *gConbineBufPtr = &gConbineBuf[0];

//---- �������ĕ�������r���邽�߂̃o�b�t�@
static char gLineBuf[PRINT_ONETIME_SIZE + 1];


void procPrintf(void)
{
    int     blankTime = 0;

    //---- �I��������̃T�C�Y
    gExitStrLength = strlen(OS_EXIT_STRING_1);

    while (1)
    {
        BOOL    isOutputString = FALSE;

        //---- ���[�U�Ɏ~�߂�ꂽ�ꍇ
        if (gStoppedByUser)
        {
            myExit(EXIT_NUM_USER_SIGNAL);
        }

        //printf("[######]\n" );
        {
            int     n;
            int     dataSize;
            char    tmpBuf[PRINT_ONETIME_SIZE + 1];

            for (n = 0; n < 2; n++)
            {
                //---- �\���f�[�^�擾
                if (!ISNTD_GetDebugPrint
                    (gDeviceHandle, archNum[n], tmpBuf, &dataSize, PRINT_ONETIME_SIZE))
                {
                    ISNTD_DeviceClose(gDeviceHandle);
                    displayErrorAndExit(EXIT_NUM_PRINTF_ERROR,
                                        "troubled while receiving print data.");
                }
                tmpBuf[dataSize] = '\0';

                //---- �\���f�[�^������Ȃ�\��
                if (dataSize)
                {
                    //---- �\�������A�ƋL��
                    isOutputString = TRUE;

                    //---- �o��
                    if (!outputString(tmpBuf, dataSize))
                    {
                        break;
                    }
                }
            }
        }

        //---- �I�����邩
        if (gStringAborted || gExitAborted)
        {
            break;
        }

        //---- �����\�����Ă��Ȃ��ꍇ
        if (!isOutputString)
        {
            Sleep(100);
            blankTime += 100;

            //---- �^�C���A�E�g����
            if (gTimeOutTime && blankTime > gTimeOutTime * 1000)
            {
                gTimeOutOccured = TRUE;
                break;
            }
        }
        //---- �����\�������ꍇ
        else
        {
            blankTime = 0;
        }

        //---- �^�C���A�E�g����
        if (gExecTimeOutTime > 0)
        {
            time_t  currentTime;
            (void)time(&currentTime);

            if (currentTime - gStartTime >= gExecTimeOutTime)
            {
                gExecTimeOutOccured = TRUE;
                break;
            }
        }
    }
}


/*---------------------------------------------------------------------------*
  Name:         showLapTime

  Description:  display lap time at line head

  Arguments:    None

  Returns:      None
 *---------------------------------------------------------------------------*/
void showLapTime(void)
{
    int     lap;
    time_t  currentTime;

    (void)time(&currentTime);
    lap = currentTime - gStartTime;

    printf("{%d:%02d}", lap / 60, lap % 60);
}

/*---------------------------------------------------------------------------*
  Name:         outputString

  Description:  output string to stdout

  Arguments:    buf     : buffer
                bufSize : data size in buffer

  Returns:      FALSE if to do quit.
 *---------------------------------------------------------------------------*/
BOOL outputString(char *buf, int bufSize)
{
    char   *bufEnd = buf + bufSize;
    char   *p = buf;

    int     abortStrLength = gAbortString ? strlen(gAbortString) : 0;

    while (p < bufEnd)
    {
        char   *crPtr = strchr(p, '\n');

        //---- \n ���Ȃ�
        if (!crPtr)
        {
            //----��r�p�ɕۑ�
            strcat(gConbineBufPtr, p);
            gConbineBufPtr += strlen(p);

            //---- �����\��
            if (!gQuietMode && gIsLineHead && gShowLapTime)
            {
                showLapTime();
            }
            gIsLineHead = FALSE;

            //---- �\��
            printfIfNotQuiet(p);

            //---- �o�b�t�@�I�[�o�[�Ȃ�j��(\n�����Ă��Ȃ��o�͑��̐ӔC)
            if (gConbineBufPtr - &gConbineBuf[0] > PRINT_ONETIME_SIZE)
            {
                gConbineBufPtr = &gConbineBuf[0];
                *gConbineBufPtr = '\0';
            }

            break;
        }

        //---- \n �܂ł��R�s�[
        {
            int     n = crPtr - p + 1;

            //---- ��r�p�Ɍ���
            strncpy(gConbineBufPtr, p, n);
            gConbineBufPtr[n] = '\0';

            //---- �\���p
            strncpy(&gLineBuf[0], p, n);
            gLineBuf[n] = '\0';
        }

        //---- �����\��
        if (!gQuietMode && gIsLineHead && gShowLapTime)
        {
            showLapTime();
        }
        gIsLineHead = TRUE;

        //---- �s�\��
        printfIfNotQuiet(gLineBuf);

        //---- �����I��������Ƃ̔�r
        if (gAbortString && !strncmp(gConbineBuf, gAbortString, abortStrLength))
        {
            gStringAborted = TRUE;
            return FALSE;
        }

        //---- OS_Exit �ɂ��I��
        if (!strncmp(gConbineBuf, OS_EXIT_STRING_1, gExitStrLength))
        {
            gExitAborted = TRUE;
            gExitStatusNum = atoi(gConbineBuf + gExitStrLength);
            return FALSE;
        }

        gConbineBufPtr = &gConbineBuf[0];
        *gConbineBufPtr = '\0';

        p = crPtr + 1;
    }

    return TRUE;
}

/*---------------------------------------------------------------------------*
  Name:         signalHandler

  Description:  signal handler

  Arguments:    sig
                argv : argument vector

  Returns:      ---
 *---------------------------------------------------------------------------*/
void signalHandler(int sig)
{
    gStoppedByUser = TRUE;
}

/*---------------------------------------------------------------------------*
  Name:         main

  Description:  main proc

  Arguments:    argc : argument count
                argv : argument vector

  Returns:      ---
 *---------------------------------------------------------------------------*/
int main(int argc, char *argv[])
{
    //---- DLL ������
    ISNTD_InitDll();

    //---- �W���o�͂̃o�b�t�@�����O�̖�����
    setvbuf(stdout, NULL, _IONBF, 0);

    //---- �I�v�V�������
    parseOption(argc, argv);

    //---- �f�o�C�X�ǂݍ���
    gConnectedDeviceNum = ISNTD_GetDeviceList(&gDeviceList[0], DEVICE_MAX_NUM);
    if (gConnectedDeviceNum < 0)
    {
        displayErrorAndExit(EXIT_NUM_NO_DEVICE, "Cannot access devices.");
    }

    //---- �f�o�C�X����
    searchDevice();

    //---- �ǂݍ���
    loadFile();

    //---- �V�O�i���ݒ�
    (void)signal(SIGINT, signalHandler);

    //---- �X���b�g
    setSlotPower();

    //---- �X�^�[�g�����擾
    (void)time(&gStartTime);

    //----printf����
    procPrintf();

    //---- �I��
    if (gExitAborted)                  //---- OS_Exit() �I��
    {
        myExit(gExitStatusNum);
    }
    else if (gStringAborted)           //---- ������I��
    {
        myExit(EXIT_NUM_STRING_ABORT);
    }
    else if (gTimeOutOccured)          //---- �^�C���A�E�g
    {
        myExit(EXIT_NUM_TIME_OUT);
    }
    else if (gExecTimeOutOccured)
    {
        myExit(EXIT_NUM_EXEC_TIME_OUT);
    }
    else                               //---- �ʏ�I��
    {
        myExit(EXIT_NUM_NO_ERROR);
    }
    //---- never reached here

    //---- dummy to avoid warning
    return 0;
}
