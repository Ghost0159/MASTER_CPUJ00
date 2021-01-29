/*---------------------------------------------------------------------------*
  Project:  NitroSDK - WM - demos - wireless-all
  File:     common.c

  Copyright 2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: common.c,v $
  Revision 1.2  2006/04/10 13:37:12  yosizaki
  beautify sourcecode indent.

  Revision 1.1  2006/04/10 13:07:32  yosizaki
  initial upload.

  $NoKeywords: $
 *---------------------------------------------------------------------------*/


#include <nitro.h>

#include "common.h"


/*****************************************************************************/
/* constant */

#define BGSTR_MAX_LENGTH        32

/* �e�L�X�g�`��p�f�[�^ */
extern const u16 _binary_bg_plt_dat[16][16];
extern const u32 _binary_bg_chr_dat[8 * 0x100];


/*****************************************************************************/
/* variable */

/* �`�����l�� */
static u16 current_channel = 0;

/* �L�[���� */
static int own_aid;
static GShareData current_input[WH_PLAYER_MAX] ATTRIBUTE_ALIGN(32);
static BOOL valid_input[WH_PLAYER_MAX];
static GShareData next_input;
static u16 padPress;
static u16 padTrig;

/* �`��p�ϐ� */
static u16 vscr[32 * 32];
static GXOamAttr oamBak[128];


/*****************************************************************************/
/* function */

/*---------------------------------------------------------------------------*
  Name:         IS_PAD_PRESS

  Description:  ���g�̃L�[����.

  Arguments:    ���肷��L�[�t���O.

  Returns:      �w�肵���L�[����������Ă���ꍇ�� TRUE,
                ����Ă��Ȃ��ꍇ�� FALSE.
 *---------------------------------------------------------------------------*/
BOOL IS_PAD_PRESS(u16 flag)
{
    return (padPress & flag) == flag;
}

/*---------------------------------------------------------------------------*
  Name:         IS_PAD_TRIGGER

  Description:  ���g�̃L�[�g���K����.

  Arguments:    ���肷��L�[�t���O.

  Returns:      �w�肵���L�[�̃g���K�������Ă���ꍇ�� TRUE.
                �����Ă��Ȃ��ꍇ�� FALSE.
 *---------------------------------------------------------------------------*/
BOOL IS_PAD_TRIGGER(u16 flag)
{
    return (padTrig & flag) == flag;
}

/*---------------------------------------------------------------------------*
  Name:         UpdateInput

  Description:  �L�[���͂̍X�V.
                ���C�����X�ʐM���m�����Ă���ꍇ�̓f�[�^�V�F�A�����O�����s��,
                �����łȂ��ꍇ�͎��g�̃L�[���͂������X�V����.

  Arguments:    None.

  Returns:      �L�[���͍X�V�ɐ��������� TRUE.
 *---------------------------------------------------------------------------*/
static BOOL UpdateInput(void)
{
    BOOL    retval = FALSE;

    {
        OSIntrMode bak_cpsr = OS_DisableInterrupts();
        next_input.key = PAD_Read();
        next_input.count = next_input.count + 1;
        /* �f�[�^�V�F�A�����O�ȊO�Ȃ玩���̃L�[���͂�ݒ� */
        if (WH_GetSystemState() != WH_SYSSTATE_DATASHARING)
        {
            own_aid = 0;
            current_input[0] = next_input;
            valid_input[0] = TRUE;
            retval = TRUE;
        }
        /* �����łȂ���΃f�[�^�V�F�A�����O�����s */
        else if (WH_StepDS(&next_input))
        {
            /* ��������ΑS���̓��̓f�[�^�𔽉f */
            u16     i;
            for (i = 0; i < WH_PLAYER_MAX; ++i)
            {
                void   *adr = WH_GetSharedDataAdr(i);
                if (adr)
                {
                    MI_CpuCopy8(adr, &current_input[i], sizeof(current_input[i]));
                    valid_input[i] = TRUE;
                }
                else
                {
                    valid_input[i] = FALSE;
                }
            }
            retval = TRUE;
            own_aid = WH_GetCurrentAid();
        }
        (void)OS_RestoreInterrupts(bak_cpsr);
    }

    /* ���͂��X�V�ł����ꍇ�̓L�[�g���K���X�V */
    if (retval && valid_input[own_aid])
    {
        padTrig = (u16)(~padPress & current_input[own_aid].key);
        padPress = current_input[own_aid].key;
    }

    return retval;
}

/*---------------------------------------------------------------------------*
  Name:         GetCurrentInput

  Description:  �w��v���C���[�̌��݂̃L�[���͂��擾.

  Arguments:    aid               �L�[���͂��擾����v���C���[.

  Returns:      �ŐV�̃f�[�^�V�F�A�����O�ɐ������Ă���ΗL���ȃA�h���X,
                �����łȂ���� NULL.
 *---------------------------------------------------------------------------*/
GShareData *GetCurrentInput(int aid)
{
    return valid_input[aid] ? &current_input[aid] : NULL;
}

/*---------------------------------------------------------------------------*
  Name:         VBlankIntr

  Description:  �L�[�g���K�擾

  Arguments:    None

  Returns:      None
 *---------------------------------------------------------------------------*/
static void VBlankIntr(void)
{
    /* OAM�X�V */
    DC_FlushRange(oamBak, sizeof(oamBak));
    MI_DmaCopy32(3, oamBak, (void *)HW_OAM, sizeof(oamBak));
    /* BG�X�V */
    DC_FlushRange(vscr, sizeof(vscr));
    GX_LoadBG0Scr(vscr, 0, sizeof(vscr));

    /* ���荞�݃`�F�b�N�t���O */
    OS_SetIrqCheckFlag(OS_IE_V_BLANK);
}

/*---------------------------------------------------------------------------*
  Name:         InitCommon

  Description:  ���ʏ������֐�

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void InitCommon(void)
{
    /* OS ������ */
    OS_Init();
    OS_InitTick();
    OS_InitAlarm();
    FX_Init();
    {
        void   *tempLo;
        OSHeapHandle hh;
        tempLo = OS_InitAlloc(OS_ARENA_MAIN, OS_GetMainArenaLo(), OS_GetMainArenaHi(), 1);
        OS_SetArenaLo(OS_ARENA_MAIN, tempLo);
        hh = OS_CreateHeap(OS_ARENA_MAIN, OS_GetMainArenaLo(), OS_GetMainArenaHi());
        if (hh < 0)
        {
            OS_TPanic("ARM9: Fail to create heap...\n");
        }
        hh = OS_SetCurrentHeap(OS_ARENA_MAIN, hh);
    }

    /* �L�[���͂̏����� */
    MI_CpuClear8(&next_input, sizeof(next_input));
    MI_CpuClear8(&current_input, sizeof(current_input));
    own_aid = 0;
    (void)UpdateInput();

    /* FS ������ */
    if (!MB_IsMultiBootChild())
    {
        static u8 fs_tablework[0x100] ATTRIBUTE_ALIGN(32);
        FS_Init(FS_DMA_NOT_USE);
        (void)FS_LoadTable(fs_tablework, sizeof(fs_tablework));
    }

    /* GX ������ */
    GX_Init();
    GX_DispOff();
    GXS_DispOff();
    GX_SetBankForLCDC(GX_VRAM_LCDC_ALL);
    MI_CpuClearFast((void *)HW_LCDC_VRAM, HW_LCDC_VRAM_SIZE);
    (void)GX_DisableBankForLCDC();
    MI_CpuFillFast((void *)HW_OAM, 192, HW_OAM_SIZE);
    MI_CpuClearFast((void *)HW_PLTT, HW_PLTT_SIZE);
    MI_CpuFillFast((void *)HW_DB_OAM, 192, HW_DB_OAM_SIZE);
    MI_CpuClearFast((void *)HW_DB_PLTT, HW_DB_PLTT_SIZE);
    /* OBJ (VRAM-B) */
    GX_SetBankForOBJ(GX_VRAM_OBJ_128_B);
    GX_SetOBJVRamModeChar(GX_OBJVRAMMODE_CHAR_1D_32K);
    MI_DmaFill32(3, oamBak, 0xC0, sizeof(oamBak));
    GX_LoadOBJ(_binary_bg_chr_dat, 0, sizeof(_binary_bg_chr_dat));
    GX_LoadOBJPltt(_binary_bg_plt_dat, 0, sizeof(_binary_bg_plt_dat));
    /* BG (VRAM-C) */
    GX_SetBankForBG(GX_VRAM_BG_128_C);
    G2_SetBG0Control(GX_BG_SCRSIZE_TEXT_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0xf800,      /* SCR �x�[�X�u���b�N 31 */
                     GX_BG_CHARBASE_0x00000,    /* CHR �x�[�X�u���b�N 0 */
                     GX_BG_EXTPLTT_01);
    G2_SetBG0Priority(0);
    G2_BG0Mosaic(FALSE);
    GX_SetGraphicsMode(GX_DISPMODE_GRAPHICS, GX_BGMODE_0, GX_BG0_AS_2D);
    G2_SetBG0Offset(0, 0);
    GX_LoadBG0Char(_binary_bg_chr_dat, 0, sizeof(_binary_bg_chr_dat));
    GX_LoadBGPltt(_binary_bg_plt_dat, 0, sizeof(_binary_bg_plt_dat));
    MI_CpuFillFast((void *)vscr, 0, sizeof(vscr));
    DC_FlushRange(vscr, sizeof(vscr));
    DC_WaitWriteBufferEmpty();
    GX_LoadBG0Scr(vscr, 0, sizeof(vscr));
    GX_SetVisiblePlane(GX_PLANEMASK_BG0 | GX_PLANEMASK_OBJ);
    GX_DispOn();
    GXS_DispOn();

    /* V�u�����N�����ݒ� */
    (void)OS_SetIrqFunction(OS_IE_V_BLANK, VBlankIntr);
    (void)OS_EnableIrqMask(OS_IE_V_BLANK);
    (void)GX_VBlankIntr(TRUE);
    (void)OS_EnableIrq();
    (void)OS_EnableInterrupts();
}

/*---------------------------------------------------------------------------*
  Name:         GetCurrentChannel

  Description:  ���ݑI�����Ă��郏�C�����X�`�����l�����擾.

  Arguments:    None

  Returns:      ���ݑI�����Ă��郏�C�����X�`�����l��.
 *---------------------------------------------------------------------------*/
u16 GetCurrentChannel(void)
{
    return current_channel;
}

/*---------------------------------------------------------------------------*
  Name:         SetCurrentChannel

  Description:  ���݂̃��C�����X�`�����l����ݒ�.

  Arguments:    channel           �ݒ肷��`�����l��.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void SetCurrentChannel(u16 channel)
{
    current_channel = channel;
}

/*---------------------------------------------------------------------------*
  Name:         LoadLinkIcon

  Description:  �����N�A�C�R����VRAM�Ƀ��[�h.

  Arguments:    id                ���[�h��̃L�����N�^ID.
                palette           ���[�h��̃p���b�g.
                level             �����N���x.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void LoadLinkIcon(int id, int palette, int level)
{
    /* *INDENT-OFF* */
    typedef struct Icon16Format
    {
        GXRgb   palette[16];
        u8      image[16 * 16 / 2];
    }
    IconFormat;
    extern const IconFormat _binary_linkimg0_dat[];
    extern const IconFormat _binary_linkimg1_dat[];
    extern const IconFormat _binary_linkimg2_dat[];
    extern const IconFormat _binary_linkimg3_dat[];
    static const IconFormat * const table[4] =
    { _binary_linkimg0_dat, _binary_linkimg1_dat, _binary_linkimg2_dat, _binary_linkimg3_dat, };
    /* *INDENT-ON* */
    GX_LoadOBJ(table[level]->image, (u32)(32 * id), 16 * 16 / 2);
    GX_LoadOBJPltt(table[level]->palette, sizeof(GXRgb) * 16 * palette,
                   sizeof(table[level]->palette));
}

/*---------------------------------------------------------------------------*
  Name:         PrintString

  Description:  BG�e�L�X�g��`��.

  Arguments:    x                 �`�� x �O���b�h�ʒu.
                y                 �`�� x �O���b�h�ʒu.
                palette           �p���b�g�C���f�b�N�X.
                format            ����������.

  Returns:      None
 *---------------------------------------------------------------------------*/
void PrintString(int x, int y, int palette, const char *format, ...)
{
    int     i;
    char    tmp[BGSTR_MAX_LENGTH + 1] = { 0, };
    va_list vlist;
    va_start(vlist, format);
    (void)OS_VSNPrintf(tmp, sizeof(tmp), format, vlist);
    va_end(vlist);
    y = y * 32 + x;
    for (i = 0; (i < BGSTR_MAX_LENGTH) && tmp[i]; ++i)
    {
        vscr[(y + i) & ((32 * 32) - 1)] = (u16)((palette << 12) | tmp[i]);
    }
}

/*---------------------------------------------------------------------------*
  Name:         WaitNextFrame

  Description:  ���̃s�N�`���[�t���[����҂��`�������������.

  Arguments:    None.

  Returns:      �L�[���͂��X�V���ꂽ�� TRUE.
 *---------------------------------------------------------------------------*/
BOOL WaitNextFrame(void)
{
    BOOL    retval;
    /* ���̃s�N�`���[�t���[����ҋ@ */
    OS_WaitVBlankIntr();
    /* �L�[���͂̍X�V */
    retval = UpdateInput();
    /* �e�L�X�g���N���A */
    MI_CpuClearFast(vscr, sizeof(vscr));
    /* �����N���x�A�C�R���̕\�� */
    LoadLinkIcon(512, PLTT_LINK_ICON, WM_GetLinkLevel());
    G2_SetOBJAttr(&oamBak[64], 3, 3, 0,
                  GX_OAM_MODE_NORMAL, FALSE, GX_OAM_EFFECT_NONE,
                  GX_OAM_SHAPE_16x16, GX_OAM_COLOR_16, 512, PLTT_LINK_ICON, 0);
    return retval;
}

/*---------------------------------------------------------------------------*
  Name:         WaitWHState

  Description:  WH �̎w��̃X�e�[�g�ɂȂ�܂őҋ@.

  Arguments:    state             �J�ڂ�҂X�e�[�g.

  Returns:      None
 *---------------------------------------------------------------------------*/
void WaitWHState(int state)
{
    while (WH_GetSystemState() != state)
    {
        (void)WaitNextFrame();
        PrintString(3, 10, PLTT_WHITE, "WH working...");
        PrintString(10, 16, PLTT_WHITE, "( to state %d )", state);
    }
}
