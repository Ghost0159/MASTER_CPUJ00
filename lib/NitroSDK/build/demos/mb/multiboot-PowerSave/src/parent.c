/*---------------------------------------------------------------------------*
  Project:  NitroSDK - MB - demos - multiboot-PowerSave
  File:     parent.c

  Copyright 2005,2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: parent.c,v $
  Revision 1.4  2006/07/19 04:17:15  yosizaki
  fix about MB_SetPowerSaveMode() call.

  Revision 1.3  2006/01/18 02:11:19  kitase_hirotake
  do-indent

  Revision 1.2  2005/11/21 10:49:02  kitase_hirotake
  OS_PrintServer �̍폜
  SVC_WaitVBlankIntr �� OS_WaitVBlankIntr �ɕύX

  Revision 1.1  2005/04/28 07:15:24  yosizaki
  initial upload.

  $NoKeywords: $
 *---------------------------------------------------------------------------*/


#include <nitro.h>

#include "common.h"
#include "dispfunc.h"


/*
 * �e�@���̊�{�I�ȏ���.
 *
 * MB���C�u�����̃T���v���̓}���`�u�[�g�@�\���g�����߁A
 * �����ʐM��(�������邢�͗L��)�ǂ����̊J���@�ނ𕡐��K�v�Ƃ��܂��B
 * $NitroSDK/bin/ARM9-TS/Release/ �f�B���N�g���ɂ���
 * mb_child.srl �v���O�������ŏI���@�ɂ�����}���`�u�[�g�q�@�Ɠ�����
 * �@�\��񋟂���T���v���ɂȂ��Ă��܂��̂ŁA
 * ���̃o�C�i�����T���v���v���O�����Ɠ������@�ő��̋@�ނɓǂݍ��݁A
 * �ꏏ�Ɏ��s���Ă��������B
 */


/******************************************************************************/
/* declaration */

static void ParentInit(void);
static void ParentDraw(void);
static void ParentUpdate(void);


/******************************************************************************/
/* constant */

/* MB ���C�u�����Ɋ��蓖�Ă� DMA �ԍ� */
#define MB_DMA_NO       2

/* �e�@���g�� GGID */
#define SDK_MAKEGGID_SYSTEM(num)    (0x003FFF00 | (num))
#define MY_GGID         SDK_MAKEGGID_SYSTEM(0x20)

/* �e�@�̏����z�M�`�����l�� */
#define PARENT_CHANNEL  13


/******************************************************************************/
/* variables */

/* MB ���C�u�����֊��蓖�Ă郏�[�N�̈� */
static u8 cwork[MB_SYSTEM_BUF_SIZE];

static u16 parent_channel = PARENT_CHANNEL;

static BOOL mb_running = FALSE;

/* ���̃f�����_�E�����[�h������v���O�������̔z�� */
static MBGameRegistry mbGameList[] = {
    {
     "/em.srl",
     L"edgeDemo",
     L"edgemarking demo",
     "/data/icon.char",
     "/data/icon.plt",
     0 /* GGID */ ,
     16 /* player-num */ ,
     },
    {
     "/pol_toon.srl",
     L"PolToon",
     L"toon rendering",
     "/data/icon.char",
     "/data/icon.plt",
     0 /* GGID */ ,
     8 /* player-num */ ,
     },
};

enum
{ GAME_PROG_MAX = sizeof(mbGameList) / sizeof(*mbGameList) };

static u8 *p_segbuf[GAME_PROG_MAX] = { NULL, };


/******************************************************************************/
/* function */

/*---------------------------------------------------------------------------*
  Name:         changeChannel

  Description:  Channel ������ύX.

  Arguments:    p_channel        �ʒm�Ώۂ̎q�@ AID.
                status           �ʒm���.
                arg              �R�[���o�b�N����.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static u16 changeChannel(u16 channel)
{
    const u16 channel_bak = channel;
    u16     channel_bmp, i;

    /* channel bitmap �擾 */
    channel_bmp = WM_GetAllowedChannel();

    /* �g�p�\��channel�������ꍇ�́AOS_Panic���R�[���D */
    if (channel_bmp == 0)
    {
        OS_Panic("No Available Parent channel\n");
    }
    /* �g�p�\��channel�����݂����ꍇ, �O��ƕʂ�channel���������� */
    for (i = 0; i < 16; i++, channel = (u16)((channel == 16) ? 1 : channel + 1))
    {
        if (channel_bmp & (1 << (channel - 1)))
        {
            if (channel_bak != channel)
            {
                break;
            }
        }

    }
    return channel;
}

/*---------------------------------------------------------------------------*
  Name:         ParentStateCallback

  Description:  MB���C�u������Ԓʒm�R�[���o�b�N.

  Arguments:    aid              �ʒm�Ώۂ̎q�@ AID.
                status           �ʒm���.
                arg              �R�[���o�b�N����.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void ParentStateCallback(u16 aid, u32 status, void *arg)
{

    switch (status)
    {
    case MB_COMM_PSTATE_INIT_COMPLETE:
        BgSetMessage(WHITE, "MB lib Init complete");
        break;

    case MB_COMM_PSTATE_CONNECTED:
        {
            WMStartParentCallback *p = (WMStartParentCallback *)arg;
            BgSetMessage(YELLOW, "Connected(%2d)%02X:%02X:%02X:%02X:%02X:%02X", aid,
                         p->macAddress[0], p->macAddress[1], p->macAddress[2],
                         p->macAddress[3], p->macAddress[4], p->macAddress[5]);
        }
        break;

    case MB_COMM_PSTATE_DISCONNECTED:
        BgSetMessage(RED, "Disconnected [%2d]", aid);
        break;

    case MB_COMM_PSTATE_KICKED:
        BgSetMessage(RED, "Entry Refused [%2d]", aid);
        break;

    case MB_COMM_PSTATE_REQ_ACCEPTED:
        BgSetMessage(YELLOW, "Download Request [%2d]", aid);
        break;

    case MB_COMM_PSTATE_SEND_PROCEED:
        BgSetMessage(CYAN, "Start Sending [%2d]", aid);
        break;

    case MB_COMM_PSTATE_SEND_COMPLETE:
        BgSetMessage(CYAN, "Send Completed [%2d]", aid);
        (void)MB_CommBootRequest(aid);
        BgSetMessage(WHITE, "-->Boot Request [%2d]", aid);
        break;

    case MB_COMM_PSTATE_BOOT_REQUEST:
        BgSetMessage(CYAN, "Send boot request [%2d]", aid);
        break;

    case MB_COMM_PSTATE_BOOT_STARTABLE:
        BgSetMessage(YELLOW, "Boot ready [%2d]", aid);
        break;

    case MB_COMM_PSTATE_REQUESTED:
        BgSetMessage(YELLOW, "Entry Requested [%2d]", aid);
        (void)MB_CommResponseRequest(aid, MB_COMM_RESPONSE_REQUEST_ACCEPT);
        BgSetMessage(WHITE, "-->Entry Accept [%2d]", aid);
        break;

    case MB_COMM_PSTATE_MEMBER_FULL:
        BgSetMessage(RED, "Entry Member full [%2d]", aid);
        break;

    case MB_COMM_PSTATE_END:
        BgSetMessage(WHITE, "MB lib End");
        mb_running = FALSE;
        break;

    case MB_COMM_PSTATE_WAIT_TO_SEND:
        BgSetMessage(CYAN, "Waiting to send [%2d]", aid);
        (void)MB_CommStartSending(aid);
        BgSetMessage(WHITE, "-->Start Sending [%2d]", aid);
        break;
    }

}

/*---------------------------------------------------------------------------*
  Name:         ParentInit

  Description:  DS�_�E�����[�h�v���C�e�@��ԏ�����.

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void ParentInit(void)
{
    int     i;
    /* DS�_�E�����[�h�v���C�e�@�̏����� */
    {
        MBUserInfo user;
        /* �ȑO�̃Z�O�����g�o�b�t�@����� */
        for (i = 0; i < GAME_PROG_MAX; ++i)
        {
            if (p_segbuf[i])
            {
                OS_Free(p_segbuf[i]), p_segbuf[i] = NULL;
            }
        }
        /* �ȓd�̓��[�h�̐ݒ�ɉ����Đe�@����I�� */
        if (g_power_save_mode)
        {
            MI_CpuCopy8(L"Power:Save", user.name, 10 * sizeof(u16));
            user.favoriteColor = OS_FAVORITE_COLOR_BLUE;
        }
        else
        {
            MI_CpuCopy8(L"Power:Full", user.name, 10 * sizeof(u16));
            user.favoriteColor = OS_FAVORITE_COLOR_YELLOW;
        }
        user.nameLength = 10;
        /* MB �̏����� */
        (void)MB_Init(cwork, &user, MY_GGID, MB_TGID_AUTO, MB_DMA_NO);
        MB_SetPowerSaveMode(g_power_save_mode);
        (void)MB_CommSetParentStateCallback(ParentStateCallback);
        (void)MB_StartParent(parent_channel);
    }

    GX_DispOn();
    GXS_DispOn();

    BgClear();
    BgSetMessage(WHITE, "Initializing Parent.");
    BgSetMessage(WHITE, "** Parameters **");
    BgSetMessage(WHITE, "channel      : %2d", parent_channel);
    BgSetMessage(WHITE, "GGID:%08x TGID:%04x", MY_GGID, MB_GetTgid());

    /* �_�E�����[�h�v���O�����t�@�C������o�^ */
    for (i = 0; i < GAME_PROG_MAX; ++i)
    {
        BOOL    succeeded = FALSE;
        FSFile  file[1];
        FS_InitFile(file);

        if (!FS_OpenFile(file, mbGameList[i].romFilePathp))
        {
            OS_TPrintf("ParentInit : file cannot open (mbGameList[i].romFilePathp=\"%s)\"\n",
                       mbGameList[i].romFilePathp ? mbGameList[i].romFilePathp : "(NULL)");
        }
        else if ((p_segbuf[i] = (u8 *)OS_Alloc(MB_SEGMENT_BUFFER_MIN)) == NULL)
        {
            OS_TPrintf("ParentInit : memory allocation failed. (%d BYTE)\n", MB_SEGMENT_BUFFER_MIN);
        }
        else if (!MB_ReadSegment(file, p_segbuf[i], MB_SEGMENT_BUFFER_MIN))
        {
            OS_TPrintf("ParentInit : failed to extract segment\n");
        }
        else if (!MB_RegisterFile(&mbGameList[i], p_segbuf[i]))
        {
            OS_TPrintf("ParentInit : failed to register file No %d\n", i);
        }
        else
        {
            BgSetMessage(LIGHT_GREEN, "Registered\"%s\"", mbGameList[i].romFilePathp);
            succeeded = TRUE;
        }

        if (FS_IsFile(file))
        {
            (void)FS_CloseFile(file);
        }
        if (!succeeded && (p_segbuf[i] != NULL))
        {
            OS_Free(p_segbuf[i]), p_segbuf[i] = NULL;
        }
    }
}

/*---------------------------------------------------------------------------*
  Name:         ParentUpdate

  Description:  DS�_�E�����[�h�v���C�e�@��ԍX�V.

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void ParentUpdate(void)
{
    const u16 keyData = ReadKeySetTrigger();

    /* SELECT ����������I�� -> �ď����� */
    if ((keyData & PAD_BUTTON_SELECT) != 0)
    {
        /* �`�����l���ύX */
        parent_channel = changeChannel(parent_channel);
        MB_End();
    }

}

/*---------------------------------------------------------------------------*
  Name:         ParentUpdate

  Description:  DS�_�E�����[�h�v���C�e�@��ԕ`��.

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void ParentDraw(void)
{
    static const char *(pstate_string[]) =
    {
    "NONE        ",
            "INIT OK     ",
            "CONNECTED   ",
            "DISCONNECTED",
            "KICKED      ",
            "ENTRY OK    ",
            "SENDING     ",
            "SEND END    ",
            "BOOT REQ    ",
            "BOOT READY  ",
            "ENTRY REQ   ", "MEMBER FULL ", "END         ", "ERROR       ", "WAIT TO SEND",};
    enum
    { PSTATE_NUM = sizeof(pstate_string) / sizeof(*pstate_string) };
    enum
    { DISP_OX = 2, DISP_OY = 3 };

    BgPrintf(DISP_OX, DISP_OY - 2, WHITE, "CH:%2d", parent_channel);
    BgPutString(DISP_OX, DISP_OY - 1, WHITE, "AID USERNAME STATE        ", 32);
    BgPutString(DISP_OX, DISP_OY + MB_MAX_CHILD + 1, WHITE, "SEL:restart", 32);

    /*
     * �_�E�����[�h�Ǘ���Ԃ̕\���X�V.
     * �擾�����X�̏�Ԃ̕s������h������, �S�̂����荞�݋֎~.
     */
    {
        OSIntrMode enabled = OS_DisableInterrupts();
        int     i;

        /* �q�@���X�g�̕\�� */
        for (i = 0; i < 15; ++i)
        {
            const u16 aid = (u16)(i + 1);
            const MBUserInfo *p = MB_CommGetChildUser(aid);
            const int state = MB_CommGetParentState(aid);

            BgPrintf(DISP_OX + 0, DISP_OY + i, WHITE, "%2d                       ", aid);
            if (p)
            {
                char    name[MB_USER_NAME_LENGTH * 2 + 1] = { 0, };
                MI_CpuCopy8(p->name, name, (u32)(p->nameLength * 2));
                BgPutString(DISP_OX + 4, DISP_OY + i, p->favoriteColor, name, 8);
            }
            if (state < PSTATE_NUM)
            {
                BgPrintf(DISP_OX + 13, DISP_OY + i, WHITE, "%s", pstate_string[state]);
            }
        }

        (void)OS_RestoreInterrupts(enabled);
    }

}

/*---------------------------------------------------------------------------*
  Name:         ParentMode

  Description:  DS�_�E�����[�h�v���C�e�@����.

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void ParentMode(void)
{
    ParentInit();
    /* ���C�����[�v */
    mb_running = TRUE;
    while (mb_running)
    {
        ParentUpdate();
        ParentDraw();

        OS_WaitVBlankIntr();
    }
}
