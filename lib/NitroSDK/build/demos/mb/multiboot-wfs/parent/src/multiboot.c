/*---------------------------------------------------------------------------*
  Project:  NitroSDK - MB - demos - multiboot-wfs
  File:     multiboot.c

  Copyright 2005 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: multiboot.c,v $
  Revision 1.2  2005/08/09 13:54:59  yosizaki
  fix gamename.

  Revision 1.1  2005/06/23 09:06:03  yosizaki
  initial upload.

  $NoKeywords: $
 *---------------------------------------------------------------------------*/


#include <nitro.h>
#include <nitro/wm.h>
#include <nitro/wbt.h>
#include <nitro/fs.h>

#include    "wfs.h"
#include    "wc.h"
#include    "mbp.h"

#include    "util.h"
#include    "common.h"


/*---------------------------------------------------------------------------*
    �����萔��`
 *---------------------------------------------------------------------------*/

/* ���̃f�����_�E�����[�h������v���O������� */
const MBGameRegistry mbGameList = {
    "data/main.srl",                   // �q�@�o�C�i���R�[�h
    (u16 *)L"MultibootWFS",            // �Q�[����
    (u16 *)L"Multiboot-WFS demo",      // �Q�[�����e����
    "data/icon.char",                  // �A�C�R���L�����N�^�f�[�^
    "data/icon.plt",                   // �A�C�R���p���b�g�f�[�^
    GGID_WBT_FS,                       // GGID
    MBP_CHILD_MAX + 1,                 // �ő�v���C�l���A�e�@�̐����܂߂��l��
};


/*---------------------------------------------------------------------------*
    �����ϐ���`
 *---------------------------------------------------------------------------*/

static void *sWmWork;

/* �`�����l���v���p�ϐ��̏����� */
static volatile BOOL sChannelBusy;
static u16 sChannel;
static u16 sChannelBusyRatio;
static u16 sChannelBitmap;


/*---------------------------------------------------------------------------*
    �֐���`
 *---------------------------------------------------------------------------*/

static void GetChannelProc(void *arg);
static void GetChannelMain(void);
static void ConnectMain(void);
static void PrintChildState(void);


/*---------------------------------------------------------------------------*
  Name:         ModeMultiboot

  Description:  DS�_�E�����[�h�v���C�e�@�����B

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void ModeMultiboot(void)
{
    /* �d�g�g�p���̍ł��Ⴂ�`�����l�������� */
    GetChannelMain();

    /* �}���`�u�[�g�z�M���� */
    ConnectMain();
}

/*---------------------------------------------------------------------------*
  Name:         GetChannelProc

  Description:  �`�����l���v���̂��߂̈�A�� WM �֐��R�[���o�b�N.
                WM_Initialize() �̃R�[���o�b�N�Ƃ��Ďw�肵���ꍇ��
                �g�p�\�ȑS�`�����l���̓d�g�����v����, �Œ�̂��̂�
                sChannel �֊i�[���Ċ�������.
                WM_End() �̃R�[���o�b�N�Ƃ��Ďw�肵���ꍇ�͂��̂܂܊�������.
                �������� sChannelBusy �� FALSE �ɂ���.

  Arguments:    arg     WM �֐��R�[���o�b�N�\����

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void GetChannelProc(void *arg)
{
    u16     channel = 1;
    u16     allowedChannel;

    WMCallback *cb = (WMCallback *)arg;

    /* WM_End() �R�[���o�b�N�Ȃ犮����ʒm */
    if (cb->apiid == WM_APIID_END)
    {
        sChannelBusy = FALSE;
        return;
    }
    /* WM_Initialize() �R�[���o�b�N�Ȃ�v���������J�n */
    else if (cb->apiid == WM_APIID_INITIALIZE)
    {
    }
    /* ����ȊO�� WM_MeasureChannel() �R�[���o�b�N */
    else
    {
        WMMeasureChannelCallback *cb = (WMMeasureChannelCallback *)arg;
        if (cb->errcode != WM_ERRCODE_SUCCESS)
        {
            OS_TPanic("WM_MeasureChannel() failed!");
        }
        channel = cb->channel;
        /* �d�g�g�p�������܂܂łōŒ�Ȃ�̗p */
        if (sChannelBusyRatio > cb->ccaBusyRatio)
        {
            sChannelBusyRatio = cb->ccaBusyRatio;
            sChannelBitmap = (u16)(1 << (channel - 1));
        }
        /* ���܂܂ł̍Œ�l�Ɠ��l�̏ꍇ���ЂƂ܂��̗p */
        else if (sChannelBusyRatio == cb->ccaBusyRatio)
        {
            sChannelBitmap |= 1 << (channel - 1);
        }
        ++channel;
    }

    /* �g�p�\�ȃ`�����l�����擾 */
    allowedChannel = WM_GetAllowedChannel();
    if (allowedChannel == 0x8000)
    {
        OS_TPanic("WM_GetAllowedChannel() failed!");
    }
    else if (allowedChannel == 0)
    {
        OS_TPanic("no available wireless channels!");
    }

    /* �S�`�����l�����v�������犮�� */
    if ((1 << (channel - 1)) > allowedChannel)
    {
        int     num = MATH_CountPopulation(sChannelBitmap);
        if (num == 0)
        {
            OS_TPanic("no available wireless channels!");
        }
        else if (num == 1)
        {
            sChannel = (u16)(31 - MATH_CountLeadingZeros(sChannelBitmap) + 1);
        }
        /* �����d�g�g�p���̃`�����l�����������݂����ꍇ�̓����_���ɑI�� */
        else
        {
            int     select = (int)(((OS_GetVBlankCount() & 0xFF) * num) / 0x100);
            int     i;
            for (i = 0; i < 16; i++)
            {
                if ((sChannelBitmap & (1 << i)) && (--select < 0))
                {
                    break;
                }
            }
            sChannel = (u16)(i + 1);
        }
        sChannelBusy = FALSE;
    }
    /* ���v���̃`�����l��������Ώ����𑱍s */
    else
    {
        while (((1 << (channel - 1)) & allowedChannel) == 0)
        {
            ++channel;
        }
        if (WM_MeasureChannel(GetChannelProc, 3, 17, channel, 30) != WM_ERRCODE_OPERATING)
        {
            OS_TPanic("WM_MeasureChannel() failed!");
        }
    }
}

/*---------------------------------------------------------------------------*
  Name:         GetChannelMain

  Description:  �d�g�g�p���̍ł��Ⴂ�`�����l�����v������B

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void GetChannelMain(void)
{
    KeyInfo key[1];

    /* �`�����l���v���p�ϐ��̏����� */
    sChannelBusy = TRUE;
    sChannel = 0;
    sChannelBusyRatio = 100 + 1;
    sChannelBitmap = 0;

    /* WM ���[�N�������̊m�ۂƃ��C�u�����N�� */
    sWmWork = OS_Alloc(WM_SYSTEM_BUF_SIZE);
    if (!sWmWork)
    {
        OS_TPanic("failed to allocate memory for WM!");
    }
    else if (WM_Initialize(sWmWork, GetChannelProc, 2) != WM_ERRCODE_OPERATING)
    {
        OS_TPanic("WM_Initialize() failed!");
    }

    /* �v�������܂őҋ@ */
    InitFrameLoop(key);
    while (sChannelBusy)
    {
        WaitForNextFrame(key);
        PrintString(4, 22, COLOR_YELLOW, " Searching Channel %c",
                    ((OS_GetVBlankCount() / 10) & 1) ? '_' : ' ');
    }

#if !defined(MBP_USING_MB_EX)
    /* ���C�u�����I���� WM ���[�N�������̉�� */
    sChannelBusy = TRUE;
    if (WM_End(GetChannelProc) != WM_ERRCODE_OPERATING)
    {
        OS_TPanic("WM_End() failed!");
    }
    while (sChannelBusy)
    {
    }
    OS_Free(sWmWork), sWmWork = NULL;
#endif

}

/*---------------------------------------------------------------------------*
  Name:         ConnectMain

  Description:  �}���`�u�[�g�Őڑ�����B

  Arguments:    tgid        �e�@�Ƃ��ċN������ꍇ��tgid���w�肵�܂�.

  Returns:      �q�@�ւ̓]���ɐ��������ꍇ�ɂ� TRUE,
                ���s������L�����Z�����ꂽ�ꍇ�ɂ�  FALSE ��Ԃ��܂��B
 *---------------------------------------------------------------------------*/
static void ConnectMain(void)
{
    KeyInfo key[1];

    MBP_Init(mbGameList.ggid, WM_GetNextTgid());

    InitFrameLoop(key);
    for (;;)
    {
        int     state;

        WaitForNextFrame(key);
        PrintString(4, 22, COLOR_YELLOW, " MB Parent ");
        state = MBP_GetState();
        /* �A�C�h����� */
        if (state == MBP_STATE_IDLE)
        {
            MBP_Start(&mbGameList, sChannel);
        }
        /* �q�@����̃G���g���[��t�� */
        else if (state == MBP_STATE_ENTRY)
        {
            PrintString(4, 22, COLOR_WHITE, " Now Accepting            ");
            /* B�{�^���Ń}���`�u�[�g�L�����Z�� */
            if (key->trg & PAD_BUTTON_B)
            {
                MBP_Cancel();
            }
            /* �G���g���[���̎q�@�����ł����݂���ΊJ�n�\�Ƃ��� */
            else if (MBP_GetChildBmp(MBP_BMPTYPE_ENTRY) ||
                     MBP_GetChildBmp(MBP_BMPTYPE_DOWNLOADING) ||
                     MBP_GetChildBmp(MBP_BMPTYPE_BOOTABLE))
            {
                PrintString(4, 22, COLOR_WHITE, " Push START Button to start   ");
                if (key->trg & PAD_BUTTON_START)
                {
                    /* �_�E�����[�h�J�n */
                    MBP_StartDownloadAll();
                }
            }
        }
        /*
         * �v���O�����z�M����.
         * �S�����_�E�����[�h�������Ă���Ȃ�΍ċN���v��.
         */
        else if (state == MBP_STATE_DATASENDING)
        {
            if (MBP_IsBootableAll())
            {
                MBP_StartRebootAll();
            }
        }
        /* �q�@�ċN���̑ҋ@���� */
        else if (state == MBP_STATE_REBOOTING)
        {
            PrintString(4, 22, COLOR_WHITE, " Rebooting now                ");
        }
        /* �q�@�ċN���̊��������� */
        else if (state == MBP_STATE_COMPLETE)
        {
            PrintString(4, 22, COLOR_WHITE, " Reconnecting now             ");
            break;
        }
        /* �G���[�������͒ʐM���L�����Z�� */
        else if (state == MBP_STATE_ERROR)
        {
            MBP_Cancel();
        }
        /* �ʐM�L�����Z�������� */
        else if (state == MBP_STATE_CANCEL)
        {
        }
        /* �ʐM�ُ�I�� */
        else if (state == MBP_STATE_STOP)
        {
            break;
        }

        /* �q�@��Ԃ�\�� */
        PrintChildState();
    }

#if defined(MBP_USING_MB_EX)
    /* ���C�u�����I���� WM ���[�N�������̉�� */
    sChannelBusy = TRUE;
    if (WM_End(GetChannelProc) != WM_ERRCODE_OPERATING)
    {
        OS_TPanic("WM_End() failed!");
    }
    while (sChannelBusy)
    {
    }
    OS_Free(sWmWork), sWmWork = NULL;
#endif

}

/*---------------------------------------------------------------------------*
  Name:         PrintChildState

  Description:  �q�@������ʂɕ\������

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void PrintChildState(void)
{
    static const char *STATE_NAME[] = {
        "NONE       ",
        "CONNECTING ",
        "REQUEST    ",
        "ENTRY      ",
        "DOWNLOADING",
        "BOOTABLE   ",
        "BOOTING    ",
    };
    enum
    {
        STATE_DISP_X = 2,
        INFO_DISP_X = 15,
        BASE_DISP_Y = 2
    };

    u16     i;

    /* �q�@���X�g�̕\�� */
    for (i = 1; i <= MBP_CHILD_MAX; i++)
    {
        const MBPChildInfo *childInfo;
        MBPChildState childState = MBP_GetChildState(i);
        const u8 *macAddr;

        SDK_ASSERT(childState < MBP_CHILDSTATE_NUM);

        // ��ԕ\��
        PrintString(STATE_DISP_X, i + BASE_DISP_Y, COLOR_WHITE, STATE_NAME[childState]);

        // ���[�U�[���\��
        childInfo = MBP_GetChildInfo(i);
        macAddr = MBP_GetChildMacAddress(i);

        if (macAddr != NULL)
        {
            PrintString(INFO_DISP_X, i + BASE_DISP_Y, COLOR_WHITE,
                        "%02x%02x%02x%02x%02x%02x",
                        macAddr[0], macAddr[1], macAddr[2], macAddr[3], macAddr[4], macAddr[5]);
        }
    }
}


/*---------------------------------------------------------------------------*
  End of file
 *---------------------------------------------------------------------------*/
