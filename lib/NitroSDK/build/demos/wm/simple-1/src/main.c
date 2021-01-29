/*---------------------------------------------------------------------------*
  Project:  NitroSDK - WM - demos - simple-1
  File:     main.c

  Copyright 2003-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: main.c,v $
  Revision 1.17  2007/11/27 06:33:32  okubata_ryoma
  Copyright fix

  Revision 1.16  2007/10/29 09:30:13  seiki_masashi
  �R�����g�̏C��

  Revision 1.15  2006/01/20 02:05:39  seiki_masashi
  WMParentParam �\���̂̃R�����g������ɍ����悤�ɏC��

  Revision 1.14  2006/01/18 02:11:29  kitase_hirotake
  do-indent

  Revision 1.13  2005/11/21 11:07:10  kitase_hirotake
  OS_InitPrintServer , OS_PrintServer , PXI_Init �̍폜
  SVC_WaitVBlankIntr �� OS_WaitVBlankIntr �ɕύX

  Revision 1.12  2005/06/27 11:10:13  yosizaki
  add comment about DC_WaitWriteBufferEmpty().

  Revision 1.11  2005/02/28 05:26:12  yosizaki
  do-indent.

  Revision 1.10  2005/02/18 12:09:33  seiki_masashi
  Copyright �\�L�̏C��

  Revision 1.9  2005/02/18 12:08:23  seiki_masashi
  warning �΍�

  Revision 1.8  2004/10/22 11:06:39  terui
  wc���W���[���𖳐����L�̈�ɐ؂�o���AI/F���ύX���ꂽ���ƂɑΉ��B

  Revision 1.7  2004/08/30 04:06:37  seiki_masashi
  small fix.

  Revision 1.6  2004/08/30 02:41:19  seiki_masashi
  small fix.

  Revision 1.5  2004/08/30 01:56:53  seiki_masashi
  Port ��M�R�[���o�b�N���g�p����悤�ɕύX

  Revision 1.4  2004/08/23 07:27:27  yasu
  Workaround for pushing A button in IPL

  Revision 1.3  2004/08/17 12:39:57  seiki_masashi
  ���M�f�[�^��32�o�C�g���E�ɔz�u

  Revision 1.2  2004/08/02 07:11:43  terui
  ���M�f�[�^�ҏW�o�b�t�@��z��ɕύX

  Revision 1.1  2004/07/23 15:28:14  terui
  �������ĐV�K�ǉ�

  Revision 1.4  2004/07/23 14:52:05  terui
  Only fix comment

  Revision 1.3  2004/07/23 01:04:02  terui
  �R�[�h�𐮗��B

  Revision 1.2  2004/07/20 07:46:40  terui
  �ڑ������̂��߁A�b��ł��A�b�v���[�h

  Revision 1.1  2004/07/15 12:48:42  terui
  initial upload

  $NoKeywords: $
 *---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*
    WM���C�u�������g�p���������ɂ��ʐM���s���P�@�\�T���v���ł��B
    �����I�Ɏ��ӂ�simple�f���Ɛڑ����A4�o�C�g�̃t���[���J�E���g�𖳐��ʐM��
    �󂯓n�����܂��B

    HOWTO:
        1. �e�@�Ƃ��ĒʐM���J�n����ꍇ�͂`�{�^���������ĉ������B
           ���ӂɓ���simple�f���̎q�@�������莟��A�����I�ɂ��̎q�@��
           �ʐM���J�n���܂��B�����ɒʐM�ł���q�@��15��܂łł��B
        2. �q�@�Ƃ��ĒʐM���J�n����ꍇ�͂a�{�^���������ĉ������B
           ���ӂɓ���simple�f���̐e�@�������莟��A�����I�ɂ��̐e�@��
           �ʐM���J�n���܂��B
        3. �ڑ����̃r�W�[��ʂ�A�ڑ�������̒ʐM��ʂɂ����Ăr�s�`�q�s�{�^����
           �������ƂŐڑ������Z�b�g����A������ʂɖ߂�܂��B
 *---------------------------------------------------------------------------*/

#include    <nitro.h>
#include    <nitro/wm.h>

#include    "font.h"
#include    "wc.h"


/*---------------------------------------------------------------------------*
    �萔��`
 *---------------------------------------------------------------------------*/
#define     KEY_REPEAT_START    25     // �L�[���s�[�g�J�n�܂ł̃t���[����
#define     KEY_REPEAT_SPAN     10     // �L�[���s�[�g�̊Ԋu�t���[����

#define     PICTURE_FRAME_PER_GAME_FRAME    1

#define     DEFAULT_GGID            0x003fff10
#define     NUM_MAX_CHILD           15
#define     DEFAULT_CHAN            1
#define     PARENT_DATA_SIZE        4
#define     CHILD_DATA_SIZE         4

static const WMParentParam defaultParameter = {
    NULL, 0, 0,
    DEFAULT_GGID,                      // �Q�[���O���[�vID
    0x0000,                            // temporary ID (����+1�����)
    1,                                 // �G���g���[���t���O
    NUM_MAX_CHILD,                     // �ő�ڑ��q�@��
    0,                                 // �}���`�u�[�g�t���O (�ʏ��0)
    0,                                 // (���g�p)
    0,                                 // �A���]���t���O (WM �͉��߂��܂��񂪁AWC �����̐ݒ�� MP �p�x�ݒ�ɔ��f���܂�)
    200,                               // beacon �Ԋu
    {0, 0, 0, 0},                      // ���[�U�[��
    {0, 0, 0, 0, 0, 0, 0, 0},          // �Q�[����
    DEFAULT_CHAN,                      // �ڑ��`�����l��
    PARENT_DATA_SIZE,                  // �e�@���M�f�[�^�T�C�Y
    CHILD_DATA_SIZE                    // �q�@���M�f�[�^�T�C�Y
};

/*---------------------------------------------------------------------------*
    �\���̒�`
 *---------------------------------------------------------------------------*/
// �L�[���͏��
typedef struct KeyInfo
{
    u16     cnt;                       // �����H���͒l
    u16     trg;                       // �����g���K����
    u16     up;                        // �����g���K����
    u16     rep;                       // �����ێ����s�[�g����

}
KeyInfo;


/*---------------------------------------------------------------------------*
    �����֐���`
 *---------------------------------------------------------------------------*/
static void ModeSelect(void);          // �e�@/�q�@ �I�����
static void ModeError(void);           // �G���[�\�����
static void ModeWorking(void);         // �r�W�[���
static void ModeParent(void);          // �e�@ �ʐM���
static void ModeChild(void);           // �q�@ �ʐM���
static void VBlankIntr(void);          // V�u�����N�����݃n���h��

// �f�[�^��M���ɌĂяo�����֐�
static void MpReceiveCallback(u16 aid, u16 *data, u16 length);

// �ėp�T�u���[�`��
static void KeyRead(KeyInfo * pKey);
static void ClearString(void);
static void PrintString(s16 x, s16 y, u8 palette, char *text, ...);
static void ColorString(s16 x, s16 y, s16 length, u8 palette);
static void InitializeAllocateSystem(void);


/*---------------------------------------------------------------------------*
    �����ϐ���`
 *---------------------------------------------------------------------------*/
static u16 gScreen[32 * 32];           // ���z�X�N���[��
static KeyInfo gKey;                   // �L�[����
static u32 gFrame;                     // �t���[���J�E���^

// �\���p����M�o�b�t�@
static u32 gSendBuf[1024] ATTRIBUTE_ALIGN(32);
static u32 gRecvBuf[1 + WM_NUM_MAX_CHILD][1024];
static BOOL gRecvFlag[1 + WM_NUM_MAX_CHILD];


/*---------------------------------------------------------------------------*
  Name:         NitroMain

  Description:  �������y�у��C�����[�v�B

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void NitroMain(void)
{
    // �e�평����
    OS_Init();
    FX_Init();
    GX_Init();
    GX_DispOff();
    GXS_DispOff();

    // �\���ݒ菉����
    GX_SetBankForLCDC(GX_VRAM_LCDC_ALL);
    MI_CpuClearFast((void *)HW_LCDC_VRAM, HW_LCDC_VRAM_SIZE);
    (void)GX_DisableBankForLCDC();
    MI_CpuFillFast((void *)HW_OAM, 192, HW_OAM_SIZE);
    MI_CpuClearFast((void *)HW_PLTT, HW_PLTT_SIZE);
    MI_CpuFillFast((void *)HW_DB_OAM, 192, HW_DB_OAM_SIZE);
    MI_CpuClearFast((void *)HW_DB_PLTT, HW_DB_PLTT_SIZE);

    // ������\���p�ɂQ�c�\���ݒ�
    GX_SetBankForBG(GX_VRAM_BG_128_A);
    G2_SetBG0Control(GX_BG_SCRSIZE_TEXT_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0xf800,      // SCR �x�[�X�u���b�N 31
                     GX_BG_CHARBASE_0x00000,    // CHR �x�[�X�u���b�N 0
                     GX_BG_EXTPLTT_01);
    G2_SetBG0Priority(0);
    G2_BG0Mosaic(FALSE);
    GX_SetGraphicsMode(GX_DISPMODE_GRAPHICS, GX_BGMODE_0, GX_BG0_AS_2D);
    GX_SetVisiblePlane(GX_PLANEMASK_BG0);
    GX_LoadBG0Char(d_CharData, 0, sizeof(d_CharData));
    GX_LoadBGPltt(d_PaletteData, 0, sizeof(d_PaletteData));
    MI_CpuFillFast((void *)gScreen, 0, sizeof(gScreen));
    DC_FlushRange(gScreen, sizeof(gScreen));
    /* DMA�����IO���W�X�^�փA�N�Z�X����̂ŃL���b�V���� Wait �͕s�v */
    // DC_WaitWriteBufferEmpty();
    GX_LoadBG0Scr(gScreen, 0, sizeof(gScreen));

    // �����ݐݒ�
    OS_SetIrqFunction(OS_IE_V_BLANK, VBlankIntr);
    (void)OS_EnableIrqMask(OS_IE_V_BLANK);
    (void)GX_VBlankIntr(TRUE);
    (void)OS_EnableIrq();
    (void)OS_EnableInterrupts();

    // ����������
    InitializeAllocateSystem();

    //********************************
    // ����������
    WcInit(&defaultParameter, FALSE, FALSE, PICTURE_FRAME_PER_GAME_FRAME);
    WcSetMpReceiveCallback(MpReceiveCallback);
    //********************************

    // LCD�\���J�n
    GX_DispOn();
    GXS_DispOn();

    // �f�o�b�O������o��
    OS_Printf("ARM9: WM simple demo started.\n");

    // �L�[���͏��擾�̋�Ăяo��(IPL �ł� A �{�^�������΍�)
    KeyRead(&gKey);

    // ���C�����[�v
    for (gFrame = 0; TRUE; gFrame++)
    {
        // �L�[���͏��擾
        KeyRead(&gKey);

        // �X�N���[���N���A
        ClearString();

        // �ʐM��Ԃɂ�菈����U�蕪��
        switch (WcGetStatus())
        {
        case WC_STATUS_READY:
            ModeSelect();
            break;
        case WC_STATUS_ERROR:
            ModeError();
            break;
        case WC_STATUS_BUSY:
            ModeWorking();
            break;
        case WC_STATUS_PARENT:
            ModeParent();
            break;
        case WC_STATUS_CHILD:
            ModeChild();
            break;
        }

        // �u�u�����N�҂�
        OS_WaitVBlankIntr();
    }
}

/*---------------------------------------------------------------------------*
  Name:         ModeSelect

  Description:  �e�@/�q�@ �I����ʂł̏����B

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void ModeSelect(void)
{
    PrintString(3, 10, 0xf, "Push A to connect as PARENT");
    PrintString(3, 12, 0xf, "Push B to connect as CHILD");

    if (gKey.trg & PAD_BUTTON_A)
    {
        //********************************
        s32     i;

        for (i = 1; i < (WM_NUM_MAX_CHILD + 1); i++)
        {
            gRecvFlag[i] = FALSE;
        }

        WcStartParent();
        //********************************
    }
    else if (gKey.trg & PAD_BUTTON_B)
    {
        //********************************
        WcStartChild();
        //********************************
    }
}

/*---------------------------------------------------------------------------*
  Name:         ModeError

  Description:  �G���[�\����ʂł̏����B

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void ModeError(void)
{
    PrintString(5, 10, 0x1, "======= ERROR! =======");
    PrintString(5, 13, 0xf, " Fatal error occured.");
    PrintString(5, 14, 0xf, "Please reboot program.");
}

/*---------------------------------------------------------------------------*
  Name:         ModeWorking

  Description:  �r�W�[��ʂł̏����B

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void ModeWorking(void)
{
    PrintString(9, 11, 0xf, "Now working...");

    if (gKey.trg & PAD_BUTTON_START)
    {
        //********************************
        WcEnd();
        //********************************
    }
}

/*---------------------------------------------------------------------------*
  Name:         ModeParent

  Description:  �e�@ �ʐM��ʂł̏����B

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void ModeParent(void)
{
    PrintString(8, 1, 0x2, "Parent mode");
    PrintString(4, 3, 0x4, "Send:     %08X", gSendBuf[0]);
    PrintString(4, 5, 0x4, "Receive:");
    {
        s32     i;

        for (i = 1; i < (WM_NUM_MAX_CHILD + 1); i++)
        {
            if (gRecvFlag[i])
            {
                PrintString(5, (s16)(6 + i), 0x4, "Child%02d: %08X", i, gRecvBuf[i][0]);
            }
            else
            {
                PrintString(5, (s16)(6 + i), 0x7, "No child");
            }
        }
    }

    if (gKey.trg & PAD_BUTTON_START)
    {
        //********************************
        WcEnd();
        //********************************
    }
}

/*---------------------------------------------------------------------------*
  Name:         ModeChild

  Description:  �q�@ �ʐM��ʂł̏����B

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void ModeChild(void)
{
    PrintString(8, 1, 0x2, "Child mode");
    PrintString(4, 3, 0x4, "Send:     %08X", gSendBuf[0]);
    PrintString(4, 5, 0x4, "Receive:");
    PrintString(5, 7, 0x4, "Parent:  %08X", gRecvBuf[0][0]);

    if (gKey.trg & PAD_BUTTON_START)
    {
        //********************************
        WcEnd();
        //********************************
    }
}

/*---------------------------------------------------------------------------*
  Name:         VBlankIntr

  Description:  �u�u�����N�����݃x�N�g���B

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void VBlankIntr(void)
{
    // �V�K�f�[�^���M�J�n
    switch (WcGetStatus())
    {
    case WC_STATUS_PARENT:
        gSendBuf[0] = gFrame;
        //********************************
        WcSetParentData(gSendBuf, PARENT_DATA_SIZE);
        //********************************
        break;
    case WC_STATUS_CHILD:
        gSendBuf[0] = gFrame;
        //********************************
        WcSetChildData(gSendBuf, CHILD_DATA_SIZE);
        //********************************
        break;
    }

    // ���z�X�N���[����VRAM�ɔ��f
    DC_FlushRange(gScreen, sizeof(gScreen));
    /* DMA�����IO���W�X�^�փA�N�Z�X����̂ŃL���b�V���� Wait �͕s�v */
    // DC_WaitWriteBufferEmpty();
    GX_LoadBG0Scr(gScreen, 0, sizeof(gScreen));

    // IRQ �`�F�b�N�t���O���Z�b�g
    OS_SetIrqCheckFlag(OS_IE_V_BLANK);
}

/*---------------------------------------------------------------------------*
  Name:         MpReceiveCallback

  Description:  MP�f�[�^��M���ɌĂяo�����֐��B

  Arguments:    aid     - ���M���q�@�� aid( 0 �̏ꍇ�͐e�@����̃f�[�^ )
                data    - ��M�f�[�^�ւ̃|�C���^ (NULL �Őؒf�ʒm)
                length  - ��M�f�[�^�̃T�C�Y

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void MpReceiveCallback(u16 aid, u16 *data, u16 length)
{
#pragma unused( length )
    SDK_MAX_ASSERT(aid, 15);

    if (data != NULL)
    {
        SDK_ASSERT(length >= 4);
        gRecvFlag[aid] = TRUE;
        // �R�s�[����2�o�C�g�A���C��(4�o�C�g�A���C���łȂ�)
        if (aid == 0)
        {
            // �e�@�����M�����ꍇ
            MI_CpuCopy8(data, &(gRecvBuf[aid][0]), PARENT_DATA_SIZE);
        }
        else
        {
            MI_CpuCopy8(data, &(gRecvBuf[aid][0]), CHILD_DATA_SIZE);
        }
    }
    else
    {
        gRecvFlag[aid] = FALSE;
    }
}

/*---------------------------------------------------------------------------*
  Name:         KeyRead

  Description:  �L�[���͏���ҏW����B
                �����g���K�A�����g���K�A�����p�����s�[�g�����o�B

  Arguments:    pKey  - �ҏW����L�[���͏��\���́B

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void KeyRead(KeyInfo * pKey)
{
    static u16 repeat_count[12];
    int     i;
    u16     r;

    r = PAD_Read();
    pKey->trg = 0x0000;
    pKey->up = 0x0000;
    pKey->rep = 0x0000;

    for (i = 0; i < 12; i++)
    {
        if (r & (0x0001 << i))
        {
            if (!(pKey->cnt & (0x0001 << i)))
            {
                pKey->trg |= (0x0001 << i);     // �����g���K
                repeat_count[i] = 1;
            }
            else
            {
                if (repeat_count[i] > KEY_REPEAT_START)
                {
                    pKey->rep |= (0x0001 << i); // �����p�����s�[�g
                    repeat_count[i] = KEY_REPEAT_START - KEY_REPEAT_SPAN;
                }
                else
                {
                    repeat_count[i]++;
                }
            }
        }
        else
        {
            if (pKey->cnt & (0x0001 << i))
            {
                pKey->up |= (0x0001 << i);      // �����g���K
            }
        }
    }
    pKey->cnt = r;                     // �����H�L�[����
}

/*---------------------------------------------------------------------------*
  Name:         ClearString

  Description:  ���z�X�N���[�����N���A����B

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void ClearString(void)
{
    MI_CpuClearFast((void *)gScreen, sizeof(gScreen));
}

/*---------------------------------------------------------------------------*
  Name:         PrintString

  Description:  ���z�X�N���[���ɕ������z�u����B�������32�����܂ŁB

  Arguments:    x       - ������̐擪��z�u���� x ���W( �~ 8 �h�b�g )�B
                y       - ������̐擪��z�u���� y ���W( �~ 8 �h�b�g )�B
                palette - �����̐F���p���b�g�ԍ��Ŏw��B
                text    - �z�u���镶����B�I�[������NULL�B
                ...     - ���z�����B

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void PrintString(s16 x, s16 y, u8 palette, char *text, ...)
{
    va_list vlist;
    char    temp[32 + 2];
    s32     i;

    va_start(vlist, text);
    (void)vsnprintf(temp, 33, text, vlist);
    va_end(vlist);

    *(u16 *)(&temp[32]) = 0x0000;
    for (i = 0;; i++)
    {
        if (temp[i] == 0x00)
        {
            break;
        }
        gScreen[((y * 32) + x + i) % (32 * 32)] = (u16)((palette << 12) | temp[i]);
    }
}

/*---------------------------------------------------------------------------*
  Name:         ColorString

  Description:  ���z�X�N���[���ɔz�u����������̐F��ύX����B

  Arguments:    x       - �F�ύX���J�n���� x ���W( �~ 8 �h�b�g )�B
                y       - �F�ύX���J�n���� y ���W( �~ 8 �h�b�g )�B
                length  - �A�����ĐF�ύX���镶�����B
                palette - �����̐F���p���b�g�ԍ��Ŏw��B

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void ColorString(s16 x, s16 y, s16 length, u8 palette)
{
    s32     i;
    u16     temp;
    s32     index;

    if (length < 0)
        return;

    for (i = 0; i < length; i++)
    {
        index = ((y * 32) + x + i) % (32 * 32);
        temp = gScreen[index];
        temp &= 0x0fff;
        temp |= (palette << 12);
        gScreen[index] = temp;
    }
}

/*---------------------------------------------------------------------------*
  Name:         InitializeAllocateSystem

  Description:  ���C����������̃A���[�i�ɂă����������ăV�X�e��������������B

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void InitializeAllocateSystem(void)
{
    void   *tempLo;
    OSHeapHandle hh;

    // OS_Init�͌Ă΂�Ă���Ƃ����O��
    tempLo = OS_InitAlloc(OS_ARENA_MAIN, OS_GetMainArenaLo(), OS_GetMainArenaHi(), 1);
    OS_SetArenaLo(OS_ARENA_MAIN, tempLo);
    hh = OS_CreateHeap(OS_ARENA_MAIN, OS_GetMainArenaLo(), OS_GetMainArenaHi());
    if (hh < 0)
    {
        OS_Panic("ARM9: Fail to create heap...\n");
    }
    hh = OS_SetCurrentHeap(OS_ARENA_MAIN, hh);
}

/*---------------------------------------------------------------------------*
  End of file
 *---------------------------------------------------------------------------*/