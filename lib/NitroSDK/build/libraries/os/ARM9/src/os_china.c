/*---------------------------------------------------------------------------*
  Project:  NitroSDK - OS
  File:     os_china.c

  Copyright 2005,2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: os_china.c,v $
  Revision 1.12  2006/01/18 02:11:30  kitase_hirotake
  do-indent

  Revision 1.11  2006/01/10 08:29:40  yosizaki
  fix graphics.

  Revision 1.10  2005/11/22 02:22:35  terui
  SVC_WaitVBlankIntr -> OS_WaitVBlankIntr

  Revision 1.9  2005/10/20 08:33:50  yosizaki
  update graphics data.

  Revision 1.8  2005/07/11 03:03:27  yosizaki
  disable sound.

  Revision 1.7  2005/06/02 07:55:17  yosizaki
  support sound.

  Revision 1.6  2005/05/27 08:56:42  yosizaki
  add OS_ShowAttentionChina.

  Revision 1.5  2005/05/14 01:10:29  terui
  �����Ő�p�̌��N��̒��ӁA���S����ʕ\������@�\��ǉ�

  Revision 1.4  2005/04/26 01:11:29  terui
  �萔��`����ύX

  Revision 1.3  2005/04/25 09:40:11  terui
  OS_InitChina����OS_Init���Ăяo���悤�ɏC���B

  Revision 1.2  2005/04/25 08:26:38  terui
  version�Z�N�V�������ɌŒ蕶����𖄂ߍ��݁A����J�֐�������J�֐��ɕύX

  Revision 1.1  2005/04/18 12:23:22  terui
  �����Ő�p��OSi_InitChina�֐���ǉ�

  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#include <nitro/os.h>
#include <nitro/gx.h>
#include <nitro/spi.h>
#include <nitro/pad.h>
#include <nitro/os/ARM9/china.h>

#include <nitro/version_begin.h>
static char checkString[] = OS_BURY_STRING_FORCHINA;
#include <nitro/version_end.h>


/*****************************************************************************/
/* constant */

/* �\���̃t�F�[�h����ђ�~�t���[���� */
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

/* ONLY_FORCHINA �ݒ� */
#define     OSi_ONLY_FORCHINA_CHAR_WIDTH    15
#define     OSi_ONLY_FORCHINA_CHAR_HEIGHT   2
#define     OSi_ONLY_FORCHINA_DOT_WIDTH     117
#define     OSi_ONLY_FORCHINA_DOT_HEIGHT    16
#define     OSi_ONLY_FORCHINA_CHAR_SIZE     240

/* �N��������ʗp�L�����N�^�f�[�^( 4 �r�b�g�� 3 �r�b�g�� 0 �Œ�p�b�N ) */
static u8 only_forChina_charData[OSi_ONLY_FORCHINA_CHAR_SIZE] ATTRIBUTE_ALIGN(4) =
{
0x00, 0x7c, 0xfe, 0xc6, 0xc6, 0xc6, 0xc6, 0xc6, 0x00, 0x8c, 0x8c, 0x9c, 0x9c, 0xbc, 0xbc, 0xec,
        0x00, 0x19, 0x19, 0x19, 0x19, 0x19, 0x19, 0x19, 0x00, 0xc3, 0xc3, 0x66, 0x66, 0x3c,
        0x3c, 0x18, 0x00, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0x00, 0xcf, 0xef, 0x60,
        0x60, 0x60, 0x67, 0x67, 0x00, 0xc7, 0xcf, 0xcc, 0xcc, 0xcc, 0xcc, 0xcc, 0x00, 0x0f,
        0x1f, 0x18, 0x18, 0x18, 0x1f, 0x0f, 0x00, 0x60, 0x60, 0x00, 0x00, 0x60, 0x60, 0x60,
        0x00, 0x7c, 0xfe, 0xc6, 0xc6, 0xc6, 0xc6, 0xc6, 0x00, 0x00, 0x00, 0x8c, 0x8c, 0x8c,
        0x8c, 0x8c, 0x00, 0x00, 0x00, 0xf1, 0xf9, 0x19, 0x19, 0xf9, 0x00, 0xc0, 0xc0, 0xc1,
        0xc3, 0xc3, 0xc3, 0xc3, 0x00, 0x87, 0xcf, 0xdc, 0xd8, 0xd8, 0xd8, 0x98, 0x00, 0x0f,
        0x0f, 0x00, 0x00, 0x00, 0x07, 0x0f, 0xc6, 0xc6, 0xc6, 0xfe, 0x7c, 0x00, 0x00, 0x00,
        0xec, 0xcc, 0xcc, 0x8c, 0x8c, 0x00, 0x00, 0x00, 0x19, 0x19, 0x19, 0xf9, 0xf9, 0x00,
        0x00, 0x00, 0x18, 0x18, 0x18, 0x19, 0x19, 0x00, 0x00, 0x00, 0xc0, 0xc0, 0xc0, 0xc0,
        0xc0, 0x00, 0x00, 0x00, 0x60, 0x60, 0x60, 0xe0, 0xc0, 0x00, 0x00, 0x00, 0xcc, 0xcc,
        0xcc, 0xcf, 0xc7, 0x00, 0x00, 0x00, 0x06, 0x0c, 0x0c, 0x18, 0x18, 0x00, 0x00, 0x00,
        0x60, 0x60, 0x60, 0x60, 0x60, 0x00, 0x00, 0x00, 0xc6, 0xc6, 0xc6, 0xfe, 0x7c, 0xe0,
        0xc0, 0x00, 0x8c, 0x8c, 0x8c, 0xfc, 0xf8, 0x01, 0x01, 0x00, 0xf9, 0x19, 0x19, 0xf9,
        0xf1, 0x00, 0x00, 0x00, 0xc3, 0xc0, 0xc0, 0xc3, 0xc3, 0x00, 0x00, 0x00, 0x18, 0x18,
        0x1c, 0xcf, 0xc7, 0x00, 0x00, 0x00, 0x0c, 0x0c, 0x0c, 0x0f, 0x07, 0x00, 0x00, 0x00};

/*****************************************************************************/
/* declaration */

static u8 *LoadImage(ImageIndex index, u32 *p_size);
static void WaitForNextFrame(void);
static void VBlankIntr(void);
static void SetISBNString(const char **isbn);

static void CheckLanguageCode(void);
static void CheckDetectFold(void);
static void DispExclusiveMessage(void);
static void DispLogoAndNotes(const char **isbn);


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
#if defined(SDK_CHINA_DATA_FROM_FILE)

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

#else

    extern u8 _binary_notes_forChina_chrData_bin[];
    extern u8 _binary_notes_forChina_chrData_bin_end[];
    extern u8 _binary_notes_forChina_scrData_bin[];
    extern u8 _binary_notes_forChina_scrData_bin_end[];
    extern u8 _binary_digit_chrData_bin[];
    extern u8 _binary_digit_chrData_bin_end[];
    extern u8 _binary_logo_forChina_chrData_bin[];
    extern u8 _binary_logo_forChina_chrData_bin_end[];
    extern u8 _binary_logo_forChina_scrData_bin[];
    extern u8 _binary_logo_forChina_scrData_bin_end[];
    extern u8 _binary_logo_forChina_palData_bin[];
    extern u8 _binary_logo_forChina_palData_bin_end[];
    /* *INDENT-OFF* */
    static u8 *(ptr_table[]) =
    {
        _binary_notes_forChina_chrData_bin,
        _binary_notes_forChina_scrData_bin,
        _binary_digit_chrData_bin,
        _binary_logo_forChina_chrData_bin,
        _binary_logo_forChina_scrData_bin,
        _binary_logo_forChina_palData_bin,
    };
    static u8 *(ptr_end_table[]) =
    {
        _binary_notes_forChina_chrData_bin_end,
        _binary_notes_forChina_scrData_bin_end,
        _binary_digit_chrData_bin_end,
        _binary_logo_forChina_chrData_bin_end,
        _binary_logo_forChina_scrData_bin_end,
        _binary_logo_forChina_palData_bin_end,
    };
    /* *INDENT-ON* */ 
    ptr = ptr_table[index];
    if (p_size)
    {
        *p_size = (u32)(ptr_end_table[index] - ptr_table[index]);
    }

#endif

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
    SVC_WaitVBlankIntr();
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
  Name:         OS_InitChina

  Description:  SDK �� OS ���C�u����������������B
                �w���n�������ł���A�v���P�[�V������p�B
                OS_Init �֐��̑���ɓ��֐����g�p����B

  Arguments:    isbn    -   ISBN �ԍ����Ɋւ��镶����z����w�肷��B
                            {
                                char    ISBN[ 13 ] ,
                                char    �����o�L��[ 12 ] ,
                                char    �V�o����(��)[ 4 ] ,
                                char    �V�o����(�E)[ 4 ]
                            }

  Returns:      None
 *---------------------------------------------------------------------------*/
void OS_InitChina(const char **isbn)
{
    SDK_REFER_SYMBOL(checkString);

    OS_Init();

    //---- Check possible to boot
    CheckLanguageCode();

    //---- Display logo and notes about health care
    OS_ShowAttentionChina(isbn);
}

/*---------------------------------------------------------------------------*
  Name:         CheckLanguageCode

  Description:  ����R�[�h�ɂ��r���N��������s�����ǂ����m�F����B

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void CheckLanguageCode(void)
{
    NVRAMConfig *src;

    src = (NVRAMConfig *)(OS_GetSystemWork()->nvramUserInfo);
    // ARM7�ɂ���āA�u�[�g���ׂ��łȂ�����R�[�h�Ɣ��f����Ă��邩�m�F
    if ((src->ncd.version == 0xff) && (src->ncd.option.language == 7))
    {
        DispExclusiveMessage();
        // Never return
    }
}

/*---------------------------------------------------------------------------*
  Name:         DispExclusiveMessage

  Description:  ����R�[�h�ɂ��r���N��������s�����|��\������B

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void DispExclusiveMessage(void)
{
    // �\����~
    reg_GX_DISPCNT = 0;
    reg_GXS_DB_DISPCNT = 0;

    // �p���[�R���g���[��������
    if (!(reg_GX_POWCNT & REG_GX_POWCNT_LCD_MASK))
    {
        // LCD �C�l�[�u���� OFF -> ON �ɕύX����ꍇ�� 100ms �҂�
        SVC_WaitByLoop(HW_CPU_CLOCK_ARM9 / 40);
    }
    reg_GX_POWCNT = (u16)(REG_GX_POWCNT_DSEL_MASK | REG_GX_POWCNT_E2DG_MASK |
                          REG_GX_POWCNT_E2DGB_MASK | REG_GX_POWCNT_LCD_MASK);
    // �}�X�^�[�P�x������
    reg_GX_MASTER_BRIGHT = 0;
    reg_GXS_DB_MASTER_BRIGHT = 0;

    // ���C�� LCD �ݒ�
    {
        reg_GX_VRAMCNT_A =
            (u8)((1 << REG_GX_VRAMCNT_A_MST_SHIFT) | (1 << REG_GX_VRAMCNT_A_E_SHIFT));
        reg_G2_BG0CNT =
            (u16)((GX_BG_SCRSIZE_TEXT_256x256 << REG_G2_BG0CNT_SCREENSIZE_SHIFT) |
                  (GX_BG_COLORMODE_16 << REG_G2_BG0CNT_COLORMODE_SHIFT) | (GX_BG_SCRBASE_0xf000 <<
                                                                           REG_G2_BG0CNT_SCREENBASE_SHIFT)
                  | (GX_BG_CHARBASE_0x00000 << REG_G2_BG0CNT_CHARBASE_SHIFT) | (GX_BG_EXTPLTT_01 <<
                                                                                REG_G2_BG0CNT_BGPLTTSLOT_SHIFT)
                  | (0 << REG_G2_BG0CNT_PRIORITY_SHIFT));
        reg_G2_BG0HOFS = (u16)(-(256 - OSi_ONLY_FORCHINA_DOT_WIDTH) / 2);
        reg_G2_BG0VOFS = (u16)(-(192 - OSi_ONLY_FORCHINA_DOT_HEIGHT) / 2 + 2);
        reg_GX_DISPCNT |= ((GX_BGMODE_0 << REG_GX_DISPCNT_BGMODE_SHIFT) |
                           (GX_PLANEMASK_BG0 << REG_GX_DISPCNT_DISPLAY_SHIFT));
        // �L�����N�^�f�[�^�� VRAM �Ƀ��[�h
        {
            MIUnpackBitsParam param = { OSi_ONLY_FORCHINA_CHAR_SIZE, 1, 4, 0, 0 };

            SVC_UnpackBits(only_forChina_charData, (u32 *)(HW_BG_VRAM + 0x20), &param);
        }
        // �X�N���[���f�[�^�� VRAM �ɐݒ�
        {
            s32     i;
            s32     j;
            u16     code = 1;
            u16    *dst = (u16 *)(HW_BG_VRAM + 0xf000);

            for (i = 0; i < OSi_ONLY_FORCHINA_CHAR_HEIGHT; i++)
            {
                for (j = 0; j < OSi_ONLY_FORCHINA_CHAR_WIDTH; j++)
                {
                    *dst++ = code++;
                }
                dst += (0x20 - OSi_ONLY_FORCHINA_CHAR_WIDTH);
            }
        }
    }

    // �T�u LCD �ݒ�
    {
        reg_GX_VRAMCNT_C =
            (u8)((4 << REG_GX_VRAMCNT_C_MST_SHIFT) | (1 << REG_GX_VRAMCNT_C_E_SHIFT));
        reg_G2S_DB_BG0CNT =
            (u16)((GX_BG_SCRSIZE_TEXT_256x256 << REG_G2S_DB_BG0CNT_SCREENSIZE_SHIFT) |
                  (GX_BG_COLORMODE_16 << REG_G2S_DB_BG0CNT_COLORMODE_SHIFT) | (GX_BG_SCRBASE_0xf000
                                                                               <<
                                                                               REG_G2S_DB_BG0CNT_SCREENBASE_SHIFT)
                  | (GX_BG_CHARBASE_0x00000 << REG_G2S_DB_BG0CNT_CHARBASE_SHIFT) | (GX_BG_EXTPLTT_01
                                                                                    <<
                                                                                    REG_G2S_DB_BG0CNT_BGPLTTSLOT_SHIFT)
                  | (0 << REG_G2S_DB_BG0CNT_PRIORITY_SHIFT));
        reg_G2S_DB_BG0OFS = 0;
        reg_GXS_DB_DISPCNT |= ((GX_BGMODE_0 << REG_GXS_DB_DISPCNT_BGMODE_SHIFT) |
                               (GX_PLANEMASK_BG0 << REG_GXS_DB_DISPCNT_DISPLAY_SHIFT));
    }

    // V �u�����N���荞�ݐݒ�
    (void)OS_SetIrqFunction(OS_IE_V_BLANK, VBlankIntr);
    (void)OS_EnableIrqMask(OS_IE_V_BLANK);
    (void)OS_EnableIrq();
    reg_GX_DISPSTAT |= REG_GX_DISPSTAT_VBI_MASK;

    // �\���J�n
    reg_GX_DISPCNT |= (u32)(GX_DISPMODE_GRAPHICS << REG_GX_DISPCNT_MODE_SHIFT);
    reg_GXS_DB_DISPCNT |= (u32)(REG_GXS_DB_DISPCNT_MODE_MASK);

    {
        u16     data = 0x001f;
        u16     target = 0x0001 << 5;
        s32     i;

        // �p���b�g�F��ԂƉ��F�̊Ԃŉ��������i�v�Ƀ��[�v
        while (TRUE)
        {
            for (i = 0; i < 31; i++)
            {
                // �t�^���m�F
                if (TRUE == PAD_DetectFold())
                {
                    // �d���f
                    (void)PM_ForceToPowerOff();
                }
                OS_WaitVBlankIntr();
                *((u16 *)(HW_BG_PLTT + 2)) = data;
                data += target;
            }
            target = (u16)(~target + 1);
        }
    }
}

/*---------------------------------------------------------------------------*
  Name:         OS_ShowAttentionChina

  Description:  �����Ή����S�A�y�ђ��Ӊ�ʂ����莞�ԕ\������B

  Arguments:    isbn    -   ISBN �ԍ����Ɋւ��镶����z��B

  Returns:      None.
 *---------------------------------------------------------------------------*/
SDK_WEAK_SYMBOL void OS_ShowAttentionChina(const char **isbn)
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
