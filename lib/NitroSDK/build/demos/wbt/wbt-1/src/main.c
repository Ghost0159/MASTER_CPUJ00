/*---------------------------------------------------------------------------*
  Project:  NitroSDK - WBT - demos - wbt-1
  File:     main.c

  Copyright 2003-2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: main.c,v $
  Revision 1.13  2006/08/01 02:38:45  yosizaki
  fix reinitialization.

  Revision 1.12  2006/01/18 02:11:19  kitase_hirotake
  do-indent

  Revision 1.11  2005/11/21 11:05:59  kitase_hirotake
  SVC_WaitVBlankIntr �� OS_WaitVBlankIntr �ɕύX

  Revision 1.10  2005/06/27 11:10:12  yosizaki
  add comment about DC_WaitWriteBufferEmpty().

  Revision 1.9  2005/06/17 06:37:11  miya
  bug fix.

  Revision 1.8  2005/02/28 05:26:01  yosizaki
  do-indent.

  Revision 1.7  2004/11/04 04:00:27  yosizaki
  fix comment.

  Revision 1.6  2004/10/31 23:53:18  yosizaki
  delete WC_DEFAULT_CHANNEL (abandoned symbol)

  Revision 1.5  2004/10/28 11:05:36  yosizaki
  change to use block-transfer callback of wc.
  fix around sub-BG VRAM mappings.

  Revision 1.4  2004/10/19 10:27:02  miya
  Port��M�R�[���o�b�N�̎g�p�ƃv�����g����NITRO��ʏ�ɕ\������悤�ɕύX

  Revision 1.3  2004/10/04 08:40:31  miya
  add WBT_CancelCurrentCommand

  Revision 1.2  2004/10/04 05:51:10  miya
  change transfer size according to the programing guideline

  Revision 1.1  2004/08/23 09:47:33  yosizaki
  (none)

  $NoKeywords: $
 *---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*
    WM���C�u�������g�p���������ɂ��ʐM���s���P�@�\�T���v���ł��B
    �����I�Ɏ��ӂ�wbt-1�f���Ɛڑ����܂��B

    HOWTO:
        1. �e�@�Ƃ��ĒʐM���J�n����ꍇ�͂`�{�^���������ĉ������B
           ���ӂɓ���wbt-1�f���̎q�@�������莟��A�����I�ɂ��̎q�@��
           �ʐM���J�n���܂��B�����ɒʐM�ł���q�@��15��܂łł��B
        2. �q�@�Ƃ��ĒʐM���J�n����ꍇ�͂a�{�^���������ĉ������B
           ���ӂɓ���wbt-1�f���̐e�@�������莟��A�����I�ɂ��̐e�@��
           �ʐM���J�n���܂��B
        3. �ڑ����̃r�W�[��ʂ�A�ڑ�������̒ʐM��ʂɂ����Ăr�s�`�q�s�{�^����
           �������ƂŐڑ������Z�b�g����A������ʂɖ߂�܂��B
 *---------------------------------------------------------------------------*/

#include <nitro.h>
#include <nitro/wm.h>
#include <nitro/wbt.h>

#include    "font.h"
#include    "text.h"
#include    "wc.h"
#include    "bt.h"


#define MIYA_PRINT 1


/*---------------------------------------------------------------------------*
    �萔��`
 *---------------------------------------------------------------------------*/
#define     KEY_REPEAT_START    25     // �L�[���s�[�g�J�n�܂ł̃t���[����
#define     KEY_REPEAT_SPAN     10     // �L�[���s�[�g�̊Ԋu�t���[����

// �Q�[�����
static const WMpparam wcDefaultParameter = {
    NULL, 0, 0,
    GGID_WBT_1,                        // �Q�[���O���[�vID
    0x0000,                            // temporary ID (����+1�����)
    1,                                 // �G���g���[���t���O
    15,                                // �ő�ڑ��q�@��
    0,                                 // �}���`�u�[�g�t���O
    0,                                 // �L�[�V�F�A�����O
    1,                                 // �A�����M�t���O
    200,                               // beacon �Ԋu
    {0, 0, 0, 0},                      // ���[�U�[��
    {0, 0, 0, 0, 0, 0, 0, 0},          // �Q�[����
    1,                                 // �ڑ��`�����l�� (�N������ MeasureChannel����)
    WC_PARENT_DATA_SIZE_MAX,           // �e�@���M�f�[�^�T�C�Y
    WC_CHILD_DATA_SIZE_MAX             // �q�@���M�f�[�^�T�C�Y
        // Reserves
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
static void ParentReceiveCallback(u16 aid, u16 *data, u16 length);
static void ChildReceiveCallback(u16 aid, u16 *data, u16 length);

// �f�[�^���M���ɌĂяo�����֐�
void    ParentSendCallback(void);
void    ChildSendCallback(void);

// �u���b�N�]����Ԓʒm�֐�
static void BlockTransferCallback(WMCallback *arg);

// �ėp�T�u���[�`��
static void KeyRead(KeyInfo * pKey);
static void ClearString(int vram_num);
static void PrintString(s16 x, s16 y, u8 palette, char *text, ...);
static void InitializeAllocateSystem(void);


/*---------------------------------------------------------------------------*
    �����ϐ���`
 *---------------------------------------------------------------------------*/
static KeyInfo gKey;                   // �L�[����
static s32 gFrame;                     // �t���[���J�E���^

// �\���p����M�o�b�t�@
static u8 gSendBuf[256] ATTRIBUTE_ALIGN(32);
static BOOL gRecvFlag[1 + WM_NUM_MAX_CHILD];

static int send_counter[16];
static int recv_counter[16];


#define PRINTF mprintf

#define VRAM_SIZE 2*32*32
static u8 g_screen[NUM_OF_SCREEN][VRAM_SIZE] ATTRIBUTE_ALIGN(32);


static TEXT_CTRL textctrl[NUM_OF_SCREEN];
TEXT_CTRL *tc[NUM_OF_SCREEN];

static int vram_num[2];
static int screen_toggle = 0;

static u32 v_blank_intr_counter = 0;
#define TEXT_HEAPBUF_SIZE 0x8000
static u8 text_heap_buffer[TEXT_HEAPBUF_SIZE];

static BOOL wbt_available = FALSE;
static u16 connected_bitmap = 0;


/*---------------------------------------------------------------------------*
  Name:         InitWireless

  Description:  ���C�����X�̏������E�ď����������B

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void InitWireless(void)
{
    //********************************
    // ����������
    connected_bitmap = 0;
    WcInit(&wcDefaultParameter, FALSE, TRUE, 1);
    WcSetBlockTransferCallback(BlockTransferCallback);
    //********************************
    if (wbt_available)
    {
        bt_stop();
        WBT_End();
        wbt_available = FALSE;
    }
}

/*---------------------------------------------------------------------------*
  Name:         NitroMain

  Description:  �������y�у��C�����[�v�B

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void NitroMain(void)
{
    int     i;

    // �e�평����
    OS_Init();
    OS_InitTick();

    // ����������
    (void)init_text_buf_sys((void *)&(text_heap_buffer[0]),
                            (void *)&(text_heap_buffer[TEXT_HEAPBUF_SIZE]));

    for (i = 0; i < NUM_OF_SCREEN; i++)
    {
        tc[i] = &(textctrl[i]);
        init_text(tc[i], (u16 *)&(g_screen[i]), 4 /* pal no. */ );
    }

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


    /* BG0�\���ݒ� */
    GX_SetBankForBG(GX_VRAM_BG_128_A);
    G2_SetBG0Control(GX_BG_SCRSIZE_TEXT_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0xf000,      // SCR base block 31
                     GX_BG_CHARBASE_0x00000,    // CHR base block 0
                     GX_BG_EXTPLTT_01);
    G2_SetBG0Priority(0);
    G2_BG0Mosaic(FALSE);
    // 2D�\���ݒ�
    GX_SetGraphicsMode(GX_DISPMODE_GRAPHICS, GX_BGMODE_0, GX_BG0_AS_2D);
    GX_LoadBG0Char(d_CharData, 0, sizeof(d_CharData));
    GX_LoadBGPltt(d_PaletteData, 0, sizeof(d_PaletteData));

    // OBJ�\���ݒ�
    GX_SetBankForOBJ(GX_VRAM_OBJ_128_B);
    GX_SetOBJVRamModeChar(GX_OBJVRAMMODE_CHAR_2D);

    GX_SetVisiblePlane(GX_PLANEMASK_BG0 | GX_PLANEMASK_OBJ);

    /* �T�uBG0�\���ݒ� */
    GX_SetBankForSubBG(GX_VRAM_SUB_BG_48_HI);
    G2S_SetBG0Control(GX_BG_SCRSIZE_TEXT_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0xb800,     // SCR �x�[�X�u���b�N 23
                      GX_BG_CHARBASE_0x00000,   // CHR �x�[�X�u���b�N 0
                      GX_BG_EXTPLTT_01);
    G2S_SetBG0Priority(0);
    G2S_BG0Mosaic(FALSE);
    GXS_SetGraphicsMode(GX_BGMODE_0);
    GXS_LoadBG0Char(d_CharData, 0, sizeof(d_CharData));
    GXS_LoadBGPltt(d_PaletteData, 0, sizeof(d_PaletteData));

    GX_SetBankForSubOBJ(GX_VRAM_SUB_OBJ_128_D);
    GXS_SetOBJVRamModeChar(GX_OBJVRAMMODE_CHAR_2D);

    GXS_SetVisiblePlane(GX_PLANEMASK_BG0 | GX_PLANEMASK_OBJ);

    G2_SetBG0Offset(0, 0);
    G2S_SetBG0Offset(0, 0);


    InitializeAllocateSystem();

    // LCD�\���J�n
    GX_DispOn();
    GXS_DispOn();

    // for V Blank
    (void)OS_SetIrqFunction(OS_IE_V_BLANK, VBlankIntr);
    (void)OS_EnableIrqMask(OS_IE_V_BLANK);
    (void)OS_EnableIrq();
    (void)OS_EnableInterrupts();
    (void)GX_VBlankIntr(TRUE);


    // �f�o�b�O������o��
    PRINTF("ARM9: WBT-1 demo started.\n");
    vram_num[0] = 1;
    vram_num[1] = 0;


    // ���C�����[�v

    for (gFrame = 0; TRUE; gFrame++)
    {
        text_buf_to_vram(tc[vram_num[0]]);
        text_buf_to_vram(tc[vram_num[1]]);

        if (gKey.trg & PAD_BUTTON_SELECT)
        {
            screen_toggle ^= 1;
        }
        if (gKey.trg & PAD_BUTTON_L)
        {
            vram_num[screen_toggle]--;
            if (vram_num[screen_toggle] < 0)
            {
                vram_num[screen_toggle] = (NUM_OF_SCREEN - 1);
            }
        }
        else if (gKey.trg & PAD_BUTTON_R)
        {
            vram_num[screen_toggle]++;
            if (vram_num[screen_toggle] > (NUM_OF_SCREEN - 1))
            {
                vram_num[screen_toggle] = 0;
            }
        }

        // �L�[���͏��擾
        KeyRead(&gKey);

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
        InitWireless();
        WBT_InitParent(BT_PARENT_PACKET_SIZE, BT_CHILD_PACKET_SIZE, bt_callback);
        WcSetMpReceiveCallback(ParentReceiveCallback);
        bt_register_blocks();
        WcStartParent();
        wbt_available = TRUE;
        //********************************
    }
    else if (gKey.trg & PAD_BUTTON_B)
    {
        //********************************
        InitWireless();
        WBT_InitChild(bt_callback);
        WcSetMpReceiveCallback(ChildReceiveCallback);
        WcStartChild();
        wbt_available = TRUE;
        //********************************
        vram_num[1] = 2;
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
    PrintString(4, 3, 0x4, "Send:     %08X", send_counter[0]);

    PrintString(4, 5, 0x4, "Receive:");
    {
        s32     i;

        for (i = 1; i < (WM_NUM_MAX_CHILD + 1); i++)
        {
            if (gRecvFlag[i])
            {
                PrintString(5, (s16)(6 + i), 0x4, "Child%02d: %08X", i, recv_counter[i]);
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
    PrintString(4, 3, 0x4, "Send:     %08X", send_counter[0]);
    PrintString(4, 5, 0x4, "Receive:");
    PrintString(5, 7, 0x4, "Parent:  %08X", recv_counter[0]);

    if (gKey.trg & PAD_BUTTON_START)
    {
        //********************************
        WcEnd();
        //********************************
    }
    else if (gKey.trg & PAD_BUTTON_Y)
    {
        bt_start();
    }
    else if (gKey.trg & PAD_BUTTON_X)
    {
        bt_stop();
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
    vu16   *ptr;
    int     i;

    DC_FlushRange((void *)&(g_screen[vram_num[0]]), VRAM_SIZE);
    /* DMA�����IO���W�X�^�փA�N�Z�X����̂ŃL���b�V���� Wait �͕s�v */
    // DC_WaitWriteBufferEmpty();
    GX_LoadBG0Scr(&(g_screen[vram_num[0]]), 0, VRAM_SIZE);

    DC_FlushRange((void *)&(g_screen[vram_num[1]]), VRAM_SIZE);
    /* DMA�����IO���W�X�^�փA�N�Z�X����̂ŃL���b�V���� Wait �͕s�v */
    // DC_WaitWriteBufferEmpty();
    GXS_LoadBG0Scr(&(g_screen[vram_num[1]]), 0, VRAM_SIZE);

    if (screen_toggle)
        ptr = (u16 *)G2S_GetBG0ScrPtr();
    else
        ptr = (u16 *)G2_GetBG0ScrPtr();

    for (i = 0; i < 32; i++)
    {
        *ptr = (u16)((2 << 12) | (0x00ff & '='));
        ptr++;
    }


    v_blank_intr_counter++;
    OS_SetIrqCheckFlag(OS_IE_V_BLANK);
}

/*---------------------------------------------------------------------------*
  Name:         ParentReceiveCallback

  Description:  �e�@�Ƃ��Ďq�@����̃f�[�^��M���ɌĂяo�����֐��B

  Arguments:    aid     - ���M���q�@�� aid
                data    - ��M�f�[�^�ւ̃|�C���^ (NULL �Őؒf�ʒm)
                length  - ��M�f�[�^�̃T�C�Y

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void ParentReceiveCallback(u16 aid, u16 *data, u16 length)
{
    if (data != NULL)
    {
        gRecvFlag[aid] = TRUE;
        // �R�s�[����2�o�C�g�A���C��(4�o�C�g�A���C���łȂ�)
        recv_counter[aid]++;
        WBT_MpParentRecvHook((u8 *)data, length, aid);
    }
    else
    {
        gRecvFlag[aid] = FALSE;
    }
}


/*---------------------------------------------------------------------------*
  Name:         ChildReceiveCallback

  Description:  �q�@�Ƃ��Đe�@����̃f�[�^��M���ɌĂяo�����֐��B

  Arguments:    aid     - ���M���e�@�� aid (��� 0)
                data    - ��M�f�[�^�ւ̃|�C���^ (NULL �Őؒf�ʒm)
                length  - ��M�f�[�^�̃T�C�Y

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void ChildReceiveCallback(u16 aid, u16 *data, u16 length)
{
    (void)aid;
    recv_counter[0]++;
    if (data != NULL)
    {
        gRecvFlag[0] = TRUE;
        // �R�s�[����2�o�C�g�A���C��(4�o�C�g�A���C���łȂ�)
        WBT_MpChildRecvHook((u8 *)data, length);
    }
    else
    {
        gRecvFlag[0] = FALSE;
    }
}

/*---------------------------------------------------------------------------*
  Name:         ChildReceiveCallback

  Description:  �u���b�N�]����Ԓʒm�֐��B

  Arguments:    arg     - �ʒm�� WM �֐��̃R�[���o�b�N�|�C���^

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void BlockTransferCallback(WMCallback *arg)
{
    WCStatus wcStatus = WcGetStatus();
    switch (arg->apiid)
    {
    case WM_APIID_START_MP:
        {                              /* MP �X�e�[�g�J�n */
            WMStartMPCallback *cb = (WMStartMPCallback *)arg;
            switch (cb->state)
            {
            case WM_STATECODE_MP_START:
                if (wcStatus == WC_STATUS_PARENT)
                {
                    ParentSendCallback();
                }
                else if (wcStatus == WC_STATUS_CHILD)
                {
                    WBT_SetOwnAid(WcGetAid());
                    mfprintf(tc[2], "aid = %d\n", WcGetAid());
                    bt_start();
                    ChildSendCallback();
                }
                break;
            }
        }
        break;
    case WM_APIID_SET_MP_DATA:
        {                              /* �P���� MP �ʐM���� */
            if (wcStatus == WC_STATUS_PARENT)
            {
                if (connected_bitmap != 0)
                {
                    ParentSendCallback();
                }
            }
            else if (wcStatus == WC_STATUS_CHILD)
            {
                ChildSendCallback();
            }
        }
        break;
    case WM_APIID_START_PARENT:
        {                              /* �V�K�̎q�@�ڑ� */
            WMStartParentCallback *cb = (WMStartParentCallback *)arg;
            if (wcStatus == WC_STATUS_PARENT)
            {
                switch (cb->state)
                {
                case WM_STATECODE_CONNECTED:
                    if (connected_bitmap == 0)
                    {
                        ParentSendCallback();
                    }
                    connected_bitmap |= (1 << cb->aid);
                    break;
                case WM_STATECODE_DISCONNECTED:
                    connected_bitmap &= ~(1 << cb->aid);
                    break;
                }
            }
        }
        break;
    }
}


/*---------------------------------------------------------------------------*
  Name:         ParentSendCallback

  Description:  �e�@�Ƃ��Ďq�@�ւ̃f�[�^���M���ɌĂяo�����֐��B

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void ParentSendCallback(void)
{
    const u16 size = (u16)WBT_MpParentSendHook(gSendBuf, WC_PARENT_DATA_SIZE_MAX);
    send_counter[0]++;
    WcSetParentData(gSendBuf, size);
}


/*---------------------------------------------------------------------------*
  Name:         ChildSendCallback

  Description:  �q�@�Ƃ��Đe�@����̃f�[�^��M���ɌĂяo�����֐��B

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void ChildSendCallback(void)
{
    const u16 size = (u16)WBT_MpChildSendHook(gSendBuf, WC_CHILD_DATA_SIZE_MAX);
    send_counter[0]++;
    WcSetChildData(gSendBuf, size);
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
static void ClearString(int vram_num)
{
    MI_DmaClear32(0, (void *)&(g_screen[vram_num]), VRAM_SIZE);
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
    (void)OS_VSNPrintf(temp, 32, text, vlist);
    va_end(vlist);
    *(u16 *)(&temp[31]) = 0x0000;
    for (i = 0;; i++)
    {
        if (temp[i] == 0x00)
        {
            break;
        }
        tc[1]->screen[(y * 32) + x + i] = (u16)((palette << 12) | temp[i]);
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

    mprintf(" arena lo = %08x\n", OS_GetMainArenaLo());
    mprintf(" arena hi = %08x\n", OS_GetMainArenaHi());

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
