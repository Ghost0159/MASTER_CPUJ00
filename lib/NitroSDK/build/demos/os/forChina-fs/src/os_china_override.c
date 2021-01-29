/*---------------------------------------------------------------------------*
  Project:  NitroSDK - demos - OS - forChina-fs
  File:     os_china_override.c

  Copyright 2005,2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: os_china_override.c,v $
  Revision 1.5  2006/01/18 02:11:20  kitase_hirotake
  do-indent

  Revision 1.4  2005/11/21 10:53:40  kitase_hirotake
  SVC_WaitVBlankIntr �� OS_WaitVBlankIntr �ɕύX

  Revision 1.3  2005/10/20 08:33:33  yosizaki
  update graphics data.

  Revision 1.2  2005/07/11 03:03:01  yosizaki
  disable sound.

  Revision 1.1  2005/06/02 07:57:47  yosizaki
  initial upload.

  $NoKeywords: $
 *---------------------------------------------------------------------------*/


#include <nitro.h>


/*****************************************************************************/
/* constant */

/* �N���\���̃t�F�[�h����ђ�~�t���[���� */
#define     OSi_FORCHINA_FADE_SPAN          32
#define     OSi_FORCHINA_DISP_SPAN          64

/* �e�[�u���ŊǗ�����e��f�[�^�C���[�W�̃C���f�b�N�X */
typedef enum
{
    IMAGE_NOTES_CHR,
    IMAGE_NOTES_SCR,
    IMAGE_NOTES_DIGIT,
    IMAGE_LOGO_CHR,
    IMAGE_LOGO_SCR,
    IMAGE_LOGO_PAL,
    IMAGE_LOGO_VOICE,
    IMAGE_MAX
}
ImageIndex;


/*****************************************************************************/
/* declaration */

static u8 *LoadImage(ImageIndex index, u32 *p_size);
static void WaitForNextFrame(void);
static void VBlankIntr(void);
static void SetISBNString(const char **isbn);


/*****************************************************************************/
/* function */

/*---------------------------------------------------------------------------*
  Name:         LoadImage

  Description:  �[���ȃT�C�Y�̃��������m�ۂ��Ďw�肵���t�@�C�������[�h.

  Arguments:    index   -   ���[�h����t�@�C���̃C���f�b�N�X.
                p_size  -   �t�@�C���T�C�Y���i�[���� u32 �ւ̃|�C���^.
                            �s�v�Ȃ� NULL ���w�肵�Ė��������邱�Ƃ��ł���.

  Returns:      �Ăяo���̓����Ŋm�ۂ���, �t�@�C�������[�h���ꂽ������.
                OS_ShowAttentionChina() �̏I�����ɂ�
                OS_ClearAlloc() �Ńq�[�v���Ɣj�����邽��,
                �����ŕԂ���郁�����̉�������͖����I�ɍs��Ȃ��Ă��ǂ�.
 *---------------------------------------------------------------------------*/
static u8 *LoadImage(ImageIndex index, u32 *p_size)
{
    void   *ptr = NULL;
    /* *INDENT-OFF* */
    static const char *(path_table[IMAGE_MAX]) =
    {
        "notes_forChina_chrData.bin",
        "notes_forChina_scrData.bin",
        "digit_chrData.bin",
        "logo_forChina_chrData.bin",
        "logo_forChina_scrData.bin",
        "logo_forChina_palData.bin",
        "logo_forChina_voice.bin",
    };
    /* *INDENT-ON* */ 
    const char *path = path_table[index];

    FSFile  file[1];

    FS_InitFile(file);
    if (!FS_OpenFile(file, path))
    {
        OS_TPanic("failed to open file \"%s\"", path);
    }
    else
    {
        u32     len = FS_GetLength(file);
        ptr = OS_Alloc(len);
        if (!ptr)
        {
            OS_TPanic("failed to allocate memory for image \"%s\"", path);
        }
        else
        {
            if (p_size)
            {
                *p_size = len;
            }
            if (FS_ReadFile(file, ptr, (int)len) != len)
            {
                OS_TPanic("failed to read file \"%s\"", path);
            }
        }
        (void)FS_CloseFile(file);
    }

    return (u8 *)ptr;
}

/*---------------------------------------------------------------------------*
  Name:         WaitForNextFrame

  Description:  �{�̊J���̏�����V�u�����N�ҋ@, ����уT�E���h�X�V�����B

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void WaitForNextFrame(void)
{
    /* �t�^���m�F */
    if (PAD_DetectFold())
    {
        PM_GoSleepMode(PM_TRIGGER_COVER_OPEN | PM_TRIGGER_CARD, 0, 0);
    }
    OS_WaitVBlankIntr();
}

/*---------------------------------------------------------------------------*
  Name:         VBlankIntr

  Description:  �r���N�������ʂɂ����� V �u�����N���荞�݃x�N�g���B

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void VBlankIntr(void)
{
    OS_SetIrqCheckFlag(OS_IE_V_BLANK);
}

/*---------------------------------------------------------------------------*
  Name:         SetISBNString

  Description:  ISBN �ԍ����� �X�N���[���f�[�^���̏���̈ʒu�ɔ��f������B

  Arguments:    isbn    -   ISBN �ԍ����Ɋւ��镶����z��B

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void SetISBNString(const char **isbn)
{
    s32     i, j;
    struct
    {
        u8      x, y;                  /* �\���擪�I�t�Z�b�g */
        u8      length;                /* �\�������� */
    }
    pos[] =
    {
        {
        80, 144, 13}
        ,                              /* ISBN */
        {
        120, 158, 12}
        ,                              /* �����o�L�� */
        {
        103, 175, 4}
        ,                              /* �V�o����(��) */
        {
        144, 175, 4}
        ,                              /* �V�o����(�E) */
    }
    ;
    const int count = sizeof(pos) / sizeof(*pos);       /* ���s�� */
    const int digit_id = 0;            /* ���\���L�����N�^�擪 ID ("0123456789- ") */

    GXOamAttr *dst = (GXOamAttr *)HW_DB_OAM;
    for (i = 0; i < count; i++)
    {
        for (j = 0; j < pos[i].length; ++j)
        {
            dst->attr01 = (u32)(((pos[i].x + j * 8) << 16) | (pos[i].y << 0));
            if (isbn[i][j] == '-')
            {
                dst->attr2 = (u16)(digit_id + 10);
            }
            else if ((isbn[i][j] >= '0') && (isbn[i][j] <= '9'))
            {
                dst->attr2 = (u16)(digit_id + isbn[i][j] - '0');
            }
            else
            {
                dst->attr2 = (u16)(digit_id + 11);
            }
            ++dst;
        }
    }
}

/*---------------------------------------------------------------------------*
  Name:         OS_ShowAttentionChina

  Description:  �����Ή����S�A�y�ђ��Ӊ�ʂ����莞�ԕ\������B

  Arguments:    isbn    -   ISBN �ԍ����Ɋւ��镶����z��B

  Returns:      None.
 *---------------------------------------------------------------------------*/
void OS_ShowAttentionChina(const char **isbn)
{
    /* ���W�X�^�ޔ�p�\���� */
    struct
    {
        u32     gx_dispcnt;
        u32     gxs_dispcnt;
        u16     gx_powcnt;
        u16     gx_dispstat;
        u16     gx_bright;
        u16     gxs_bright;
        u16     gx_bg0cnt;
        u16     gxs_bg0cnt;
        u32     gx_bg0ofs;
        u32     gxs_bg0ofs;
        u8      gx_vramcnt_a;
        u8      gx_vramcnt_c;
        u8      gx_vramcnt_d;
        u8      reserved[1];

        OSIrqFunction irqFunction;
        OSIrqMask irqMask;
        BOOL    irq;
        OSIntrMode interrupts;

    }
    shelter;

    /* �O���� */
    {
        /* �e���W�X�^�ޔ� */
        shelter.gx_dispstat = reg_GX_DISPSTAT;
        shelter.gx_dispcnt = reg_GX_DISPCNT;
        shelter.gxs_dispcnt = reg_GXS_DB_DISPCNT;
        shelter.gx_bright = reg_GX_MASTER_BRIGHT;
        shelter.gxs_bright = reg_GXS_DB_MASTER_BRIGHT;
        shelter.gx_powcnt = reg_GX_POWCNT;
        shelter.gx_vramcnt_a = reg_GX_VRAMCNT_A;
        shelter.gx_vramcnt_c = reg_GX_VRAMCNT_C;
        shelter.gx_vramcnt_d = reg_GX_VRAMCNT_D;
        shelter.gx_bg0cnt = reg_G2_BG0CNT;
        shelter.gxs_bg0cnt = reg_G2S_DB_BG0CNT;
        shelter.gx_bg0ofs = reg_G2_BG0OFS;
        shelter.gxs_bg0ofs = reg_G2S_DB_BG0OFS;

        /* �\����~ */
        reg_GX_DISPCNT = 0;
        reg_GXS_DB_DISPCNT = 0;

        /* �p���[�R���g���[�������� */
        if (!(shelter.gx_powcnt & REG_GX_POWCNT_LCD_MASK))
        {
            /* LCD �C�l�[�u���� OFF -> ON �ɕύX����ꍇ�� 100ms �҂� */
            SVC_WaitByLoop(HW_CPU_CLOCK_ARM9 / 40);
        }
        reg_GX_POWCNT = (u16)(REG_GX_POWCNT_DSEL_MASK | REG_GX_POWCNT_E2DG_MASK |
                              REG_GX_POWCNT_E2DGB_MASK | REG_GX_POWCNT_LCD_MASK);

        /* �}�X�^�[�P�x������ */
        reg_GX_MASTER_BRIGHT = (u16)((1 << REG_GX_MASTER_BRIGHT_E_MOD_SHIFT) | 16);
        reg_GXS_DB_MASTER_BRIGHT = reg_GX_MASTER_BRIGHT;

        /* �������Ǘ������ FS �̏����� */
        {
            void   *tmp;
            OSHeapHandle hh;
            tmp = OS_InitAlloc(OS_ARENA_MAIN, OS_GetMainArenaLo(), OS_GetMainArenaHi(), 1);
            OS_SetArenaLo(OS_ARENA_MAIN, tmp);
            hh = OS_CreateHeap(OS_ARENA_MAIN, OS_GetMainArenaLo(), OS_GetMainArenaHi());
            hh = OS_SetCurrentHeap(OS_ARENA_MAIN, hh);
            FS_Init(FS_DMA_NOT_USE);
        }

        /* Logo ��ʕ\������ */
        {
            reg_GX_VRAMCNT_A =
                (u8)((1 << REG_GX_VRAMCNT_A_MST_SHIFT) | (1 << REG_GX_VRAMCNT_A_E_SHIFT));
            reg_G2_BG0CNT =
                (u16)((GX_BG_SCRSIZE_TEXT_256x256 << REG_G2_BG0CNT_SCREENSIZE_SHIFT) |
                      (GX_BG_COLORMODE_256 << REG_G2_BG0CNT_COLORMODE_SHIFT) | (GX_BG_SCRBASE_0xf000
                                                                                <<
                                                                                REG_G2_BG0CNT_SCREENBASE_SHIFT)
                      | (GX_BG_CHARBASE_0x00000 << REG_G2_BG0CNT_CHARBASE_SHIFT) | (GX_BG_EXTPLTT_01
                                                                                    <<
                                                                                    REG_G2_BG0CNT_BGPLTTSLOT_SHIFT)
                      | (0 << REG_G2_BG0CNT_PRIORITY_SHIFT));
            reg_G2_BG0OFS = 0;
            reg_GX_DISPCNT |= ((GX_BGMODE_0 << REG_GX_DISPCNT_BGMODE_SHIFT) |
                               (GX_PLANEMASK_BG0 << REG_GX_DISPCNT_DISPLAY_SHIFT));

            /* �L�����N�^�f�[�^��VRAM�Ƀ��[�h */
            {
                void   *const src = LoadImage(IMAGE_LOGO_CHR, NULL);
                MI_UncompressLZ16(src, (u32 *)HW_BG_VRAM);
            }
            /* �X�N���[���f�[�^��VRAM�Ƀ��[�h */
            {
                u8     *const src = LoadImage(IMAGE_LOGO_SCR, NULL);
                s32     i;
                s32     j;
                u16     temp;

                SVC_CpuClearFast(0, (u32 *)(HW_BG_VRAM + 0xf000), 1536);
                for (i = 0; i < 12; i++)
                {
                    for (j = 0; j < 12; j++)
                    {
                        temp = (u16)(src[(i * 12) + j]);
                        *(u16 *)(HW_BG_VRAM + 0xf194 + (i * 64) + (j * 2)) = temp;
                    }
                }
            }
            /* �p���b�g�f�[�^��VRAM�Ƀ��[�h */
            {
                u32     size;
                void   *const src = LoadImage(IMAGE_LOGO_PAL, &size);
                SVC_CpuCopyFast(src, (u32 *)(HW_BG_PLTT), size);
            }
        }

        /* Notes ��ʕ\������ */
        {
            reg_GX_VRAMCNT_C =
                (u8)((4 << REG_GX_VRAMCNT_C_MST_SHIFT) | (1 << REG_GX_VRAMCNT_C_E_SHIFT));
            reg_GX_VRAMCNT_D =
                (u8)((4 << REG_GX_VRAMCNT_D_MST_SHIFT) | (1 << REG_GX_VRAMCNT_D_E_SHIFT));
            reg_G2S_DB_BG0CNT =
                (u16)((GX_BG_SCRSIZE_TEXT_256x256 << REG_G2S_DB_BG0CNT_SCREENSIZE_SHIFT) |
                      (GX_BG_COLORMODE_16 << REG_G2S_DB_BG0CNT_COLORMODE_SHIFT) |
                      (GX_BG_SCRBASE_0xf000 << REG_G2S_DB_BG0CNT_SCREENBASE_SHIFT) |
                      (GX_BG_CHARBASE_0x00000 << REG_G2S_DB_BG0CNT_CHARBASE_SHIFT) |
                      (GX_BG_EXTPLTT_01 << REG_G2S_DB_BG0CNT_BGPLTTSLOT_SHIFT) | (0 <<
                                                                                  REG_G2S_DB_BG0CNT_PRIORITY_SHIFT));
            reg_G2S_DB_BG0OFS = 0;
            reg_GXS_DB_DISPCNT |= ((GX_BGMODE_0 << REG_GXS_DB_DISPCNT_BGMODE_SHIFT) |
                                   ((GX_PLANEMASK_BG0 | GX_PLANEMASK_OBJ) <<
                                    REG_GXS_DB_DISPCNT_DISPLAY_SHIFT));

            /* �L�����N�^�f�[�^��VRAM�Ƀ��[�h */
            {
                u32     size;
                void   *src;
                MIUnpackBitsParam param = { 0, 1, 4, 0, 0 };
                /* BG �e�L�X�g */
                src = LoadImage(IMAGE_NOTES_CHR, &size);
                param.srcNum = (u16)size;
                SVC_UnpackBits(src, (u32 *)(HW_DB_BG_VRAM), &param);
                /* OBJ �e�L�X�g */
                src = LoadImage(IMAGE_NOTES_DIGIT, &size);
                param.srcNum = (u16)size;
                SVC_UnpackBits(src, (u32 *)(HW_DB_OBJ_VRAM), &param);
                SVC_CpuClear(0xC0, (void *)HW_DB_OAM, HW_OAM_SIZE, 32);
            }
            /* �X�N���[���f�[�^��VRAM�Ƀ��[�h */
            {
                void   *const src = LoadImage(IMAGE_NOTES_SCR, NULL);
                MI_UncompressLZ16(src, (u32 *)(HW_DB_BG_VRAM + 0xf000));
            }
            SetISBNString(isbn);
            /* �p���b�g�ҏW */
            *((u16 *)(HW_DB_BG_PLTT + 0)) = 0x0000;
            *((u16 *)(HW_DB_BG_PLTT + 2)) = 0x7fff;
            *((u16 *)(HW_DB_OBJ_PLTT + 0)) = 0x7fff;
            *((u16 *)(HW_DB_OBJ_PLTT + 2)) = 0x0000;
        }

        /* �\���J�n */
        reg_GX_DISPCNT |= (u32)(GX_DISPMODE_GRAPHICS << REG_GX_DISPCNT_MODE_SHIFT);
        reg_GXS_DB_DISPCNT |= (u32)(REG_GXS_DB_DISPCNT_MODE_MASK);

        /* ���荞�݊֘A �ݒ� */
        reg_GX_DISPSTAT |= REG_GX_DISPSTAT_VBI_MASK;
        shelter.irqFunction = OS_GetIrqFunction(OS_IE_V_BLANK);
        OS_SetIrqFunction(OS_IE_V_BLANK, VBlankIntr);
        shelter.irqMask = OS_EnableIrqMask(OS_IE_V_BLANK);
        shelter.irq = OS_EnableIrq();
        shelter.interrupts = OS_EnableInterrupts();

    }

    /* �又�� */
    {
        s32     i;
        u16     pad_old;
        u16     pad;

        /* �t�F�[�h�C�� */
        for (i = 0; i < OSi_FORCHINA_FADE_SPAN; i++)
        {
            reg_GX_MASTER_BRIGHT = (u16)((1 << REG_GX_MASTER_BRIGHT_E_MOD_SHIFT) |
                                         16 - ((i + 1) * 16 / OSi_FORCHINA_FADE_SPAN));
            reg_GXS_DB_MASTER_BRIGHT = reg_GX_MASTER_BRIGHT;
            WaitForNextFrame();
        }

        /* �{�^�����͂ɂ��X�L�b�v */
        pad_old = PAD_Read();
        for (i = 0; i < OSi_FORCHINA_DISP_SPAN; i++)
        {
            pad = PAD_Read();
            if (~pad_old & pad)
            {
                /* �C�ӂ̃{�^�������g���K�����o�����ꍇ�A�X�L�b�v */
                break;
            }
            pad_old = pad;
            WaitForNextFrame();
        }

        /* �t�F�[�h�A�E�g */
        for (i = 0; i < OSi_FORCHINA_FADE_SPAN; i++)
        {
            reg_GX_MASTER_BRIGHT = (u16)((1 << REG_GX_MASTER_BRIGHT_E_MOD_SHIFT) |
                                         (i + 1) * 16 / OSi_FORCHINA_FADE_SPAN);
            reg_GXS_DB_MASTER_BRIGHT = reg_GX_MASTER_BRIGHT;
            WaitForNextFrame();
        }
    }

    /* �㏈�� */
    {

        /* FS ����у������Ǘ��̏I�� */
        {
            FS_End();
            OS_ClearAlloc(OS_ARENA_MAIN);
        }

        /* �\����~ */
        reg_GX_DISPCNT &= ~(GX_DISPMODE_GRAPHICS << REG_GX_DISPCNT_MODE_SHIFT);
        reg_GXS_DB_DISPCNT &= ~REG_GXS_DB_DISPCNT_MODE_MASK;

        /* ���荞�݊֘A ���A */
        (void)OS_RestoreInterrupts(shelter.interrupts);
        (void)OS_RestoreIrq(shelter.irq);
        (void)OS_SetIrqMask(shelter.irqMask);
        OS_SetIrqFunction(OS_IE_V_BLANK, shelter.irqFunction);
        reg_GX_DISPSTAT &= ~REG_GX_DISPSTAT_VBI_MASK;

        /* �g�p���� VRAM ���N���A ( �T�C�Y���傫���̂ŏ����l�̕��A�͂ł��Ȃ� ) */
        SVC_CpuClearFast(0, (u32 *)HW_BG_VRAM, 0x10000);
        SVC_CpuClearFast(0, (u32 *)HW_BG_PLTT, 0x200);
        SVC_CpuClearFast(0, (u32 *)HW_DB_BG_VRAM, 0x10000);
        SVC_CpuClearFast(0, (u32 *)HW_DB_BG_PLTT, 0x04);
        SVC_CpuClearFast(0, (u32 *)HW_DB_OAM, HW_OAM_SIZE);

        /* �e���W�X�^���A */
        reg_G2S_DB_BG0OFS = shelter.gxs_bg0ofs;
        reg_G2_BG0OFS = shelter.gx_bg0ofs;
        reg_G2S_DB_BG0CNT = shelter.gxs_bg0cnt;
        reg_G2_BG0CNT = shelter.gx_bg0cnt;
        reg_GX_VRAMCNT_D = shelter.gx_vramcnt_d;
        reg_GX_VRAMCNT_C = shelter.gx_vramcnt_c;
        reg_GX_VRAMCNT_A = shelter.gx_vramcnt_a;
        reg_GX_POWCNT = shelter.gx_powcnt;
        if (!(shelter.gx_powcnt & REG_GX_POWCNT_LCD_MASK))
        {
            /* LCD �C�l�[�u���� ON -> OFF �ɕύX�����ꍇ�� 100ms �҂� */
            SVC_WaitByLoop(HW_CPU_CLOCK_ARM9 / 40);
        }
        reg_GXS_DB_MASTER_BRIGHT = shelter.gxs_bright;
        reg_GX_MASTER_BRIGHT = shelter.gx_bright;
        reg_GXS_DB_DISPCNT = shelter.gxs_dispcnt;
        reg_GX_DISPCNT = shelter.gx_dispcnt;
        reg_GX_DISPSTAT = shelter.gx_dispstat;
    }
}

/*---------------------------------------------------------------------------*
    End of file
 *---------------------------------------------------------------------------*/
