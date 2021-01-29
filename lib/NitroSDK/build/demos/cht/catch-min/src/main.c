/*---------------------------------------------------------------------------*
  Project:  NitroSDK - CHT - demos - catch-min
  File:     main.c

  Copyright 2003-2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: main.c,v $
  Revision 1.3  2006/01/18 02:11:20  kitase_hirotake
  do-indent

  Revision 1.2  2005/11/21 10:20:04  kitase_hirotake
  OS_InitPrintServer , PXI_Init �̍폜
  SVC_WaitVBlankIntr �� OS_WaitVBlankIntr �ɕύX

  Revision 1.1  2005/07/14 01:34:48  kitase_hirotake
  Initial Release

  Revision 1.1  2004/12/22 02:42:43  terui
  Initial upload

  $NoKeywords: $
 *---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*
    �s�N�g�L���b�`�@�\�ɂ��A�s�N�g�`���b�g�e�@�̃r�[�R�������o����f���ł��B
 *---------------------------------------------------------------------------*/

#include    <nitro.h>
#include    <nitro/wm.h>
#include    <nitro/cht.h>

#include    "font.h"
#include    "icon.h"
#include    "wmscan.h"

/*---------------------------------------------------------------------------*
    �萔��`
 *---------------------------------------------------------------------------*/
#define     KEY_REPEAT_START    25     // �L�[���s�[�g�J�n�܂ł̃t���[����
#define     KEY_REPEAT_SPAN     10     // �L�[���s�[�g�̊Ԋu�t���[����

#define     DEFAULT_GGID        0x003fff61

#define     PICTO_CATCH_LIFETIME    1200        // 20�b
#define     PICTO_SCAN_FREQUENCE    20 //  1/3�b (3�`�����l����1�b)

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

typedef struct PictoCatchInfo
{
    int     lifeTime;
    int     clientNum;

}
PictoCatchInfo;


/*---------------------------------------------------------------------------*
    �����֐���`
 *---------------------------------------------------------------------------*/
// �e�@�����R�[���o�b�N
static void FoundParentCallback(WMBssDesc *pBssDesc);

// V�u�����N�����݃n���h��
static void VBlankIntr(void);

// �ėp�T�u���[�`��
static void KeyRead(KeyInfo * pKey);
static void ClearString(void);
static void PrintString(s16 x, s16 y, u8 palette, char *text, ...);
static void ColorString(s16 x, s16 y, s16 length, u8 palette);
static void InitializeAllocateSystem(void);
static void DrawIcon(u8 index, int charName, int x, int y);


/*---------------------------------------------------------------------------*
    �����ϐ���`
 *---------------------------------------------------------------------------*/
static u16 gScreen[32 * 32] ATTRIBUTE_ALIGN(32);
static GXOamAttr gOam[128] ATTRIBUTE_ALIGN(32);
static KeyInfo gKey;                   // �L�[����
static s32 gFrame;                     // �t���[���J�E���^
static s32 gScanCount;

static PictoCatchInfo gRoom[4];


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
    GX_SetBankForOBJ(GX_VRAM_OBJ_16_F);
    GX_SetOBJVRamModeChar(GX_OBJVRAMMODE_CHAR_1D_32K);
    G2_SetBG0Priority(0);
    G2_BG0Mosaic(FALSE);
    GX_SetGraphicsMode(GX_DISPMODE_GRAPHICS, GX_BGMODE_0, GX_BG0_AS_2D);
    GX_SetVisiblePlane(GX_PLANEMASK_BG0 | GX_PLANEMASK_OBJ);
    GX_LoadBG0Char(d_CharData, 0, sizeof(d_CharData));
    GX_LoadBGPltt(d_PaletteData, 0, sizeof(d_PaletteData));
    GX_LoadOBJ(icons_character, 0, sizeof(icons_character));
    GX_LoadOBJPltt(icons_palette, 0, sizeof(icons_palette));
    MI_CpuFillFast((void *)gScreen, 0, sizeof(gScreen));
    DC_FlushRange(gScreen, sizeof(gScreen));
    GX_LoadBG0Scr(gScreen, 0, sizeof(gScreen));

    // �����ݐݒ�
    OS_SetIrqFunction(OS_IE_V_BLANK, VBlankIntr);
    (void)OS_EnableIrqMask(OS_IE_V_BLANK);
    (void)GX_VBlankIntr(TRUE);
    (void)OS_EnableIrq();
    (void)OS_EnableInterrupts();

    // ����������
    InitializeAllocateSystem();

    while (1)
    {
        u8     *wmbuf;

        //********************************
        // ����������
        wmbuf = (u8 *)OS_Alloc(WM_SYSTEM_BUF_SIZE);
        OS_TPrintf("Alloc %d byte\n", WM_SYSTEM_BUF_SIZE);

        if (!WS_Initialize(wmbuf, 3))
        {
            OS_TPanic("WS_Initialize failed.\n");
        }
        WS_TurnOnPictoCatch();

        {
            const u8 mac[6] = { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };
            s32     i;

            for (i = 0; i < 4; i++)
            {
                gRoom[i].lifeTime = 0;
            }
            // �����������҂�
            while (WS_GetSystemState() != WS_SYSSTATE_IDLE)
            {
            }
            // �e�@�T���J�n
            if (!WS_StartScan(FoundParentCallback, mac, FALSE))
            {
                OS_TPanic("WS_StartScan failed.\n");
            }
            gScanCount = 0;
        }
        //********************************

        // LCD�\���J�n
        GX_DispOn();
        GXS_DispOn();

        // �f�o�b�O������o��
        OS_Printf("ARM9: CHT catch-min demo started.\n");

        // �L�[���͏��擾�̋�Ăяo��(IPL �ł� A �{�^�������΍�)
        KeyRead(&gKey);

        // ���C�����[�v
        for (gFrame = 0; TRUE; gFrame++)
        {
            // �L�[���͏��擾
            KeyRead(&gKey);

            if (++gScanCount == PICTO_SCAN_FREQUENCE)
            {
                const u8 mac[6] = { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };
                if (!WS_StartScan(FoundParentCallback, mac, FALSE))
                {
                    OS_TPanic("WS_StartScan failed.\n");
                }
                gScanCount = 0;
            }

            // �X�N���[���N���A
            ClearString();

            // �\��
            PrintString(1, 1, 0xf, "frame: %d", gFrame);
            {
                s32     i;

                for (i = 0; i < 4; i++)
                {
                    if (gRoom[i].lifeTime > 0)
                    {
                        gRoom[i].lifeTime--;
                        PrintString(1, (s16)(6 + (3 * i)), 0xf, "Discover pictochat room%d", i);
                        PrintString(2, (s16)(7 + (3 * i)), 0x2, "%d members", gRoom[i].clientNum);
                        // �A�C�R����\��
                        DrawIcon((u8)i, 9, 208, (s16)(48 + (24 * i)));
                    }
                    else
                    {
                        PrintString(10, (s16)(6 + (3 * i)), 0xe, "pictochat room%d", i);
                    }
                }
            }

            if (gKey.trg == PAD_BUTTON_B)
            {
                break;
            }

            // �u�u�����N�҂�
            OS_WaitVBlankIntr();
            // ARM7�̃f�o�b�O�v�����g���o��
        }

        // �s�N�g�L���b�`�I������
        if (!WS_EndScan())
        {
            OS_TPanic("WS_EndScan failed.\n");
        }
        while (WS_GetSystemState() != WS_SYSSTATE_IDLE)
        {
        }

        if (!WS_End())
        {
            OS_TPanic("WS_End failed.\n");
        }
        while (WS_GetSystemState() != WS_SYSSTATE_STOP)
        {
        }
        OS_Free(wmbuf);
    }
}

/*---------------------------------------------------------------------------*
  Name:         FoundParentCallback

  Description:  �e�@�������ɌĂяo�����R�[���o�b�N�֐��B

  Arguments:    pBssDesc    -   ���������e�@�̃r�[�R�����ւ̃|�C���^�B

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void FoundParentCallback(WMBssDesc *pBssDesc)
{
    if (CHT_IsPictochatParent(pBssDesc))
    {
        int     clientNum;
        int     roomNumber;

        clientNum = CHT_GetPictochatClientNum(pBssDesc);
        roomNumber = CHT_GetPictochatRoomNumber(pBssDesc);
        if (roomNumber < 4)
        {
            if (gRoom[roomNumber].lifeTime == 0)
            {
                /* SE should start here */
            }
            gRoom[roomNumber].clientNum = clientNum;
            gRoom[roomNumber].lifeTime = PICTO_CATCH_LIFETIME;
        }
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
    // ���z�X�N���[����VRAM�ɔ��f
    DC_FlushRange(gScreen, sizeof(gScreen));
    GX_LoadBG0Scr(gScreen, 0, sizeof(gScreen));

    // ���z OAM �� OAM �ɔ��f
    DC_FlushRange(gOam, sizeof(gOam));
    GX_LoadOAM(gOam, 0, sizeof(gOam));

    // IRQ �`�F�b�N�t���O���Z�b�g
    OS_SetIrqCheckFlag(OS_IE_V_BLANK);
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
    s32     i;

    MI_CpuClearFast((void *)gScreen, sizeof(gScreen));
    for (i = 0; i < 128; i++)
    {
        G2_SetOBJPosition(&gOam[i], 256, 192);
    }
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
  Name:         DrawIcon

  Description:  �A�C�R����\������B

  Arguments:    index       - ���z OAM �̔z�� index ���w��B
                charName    - �A�C�R���ԍ����w��B
                                0 �` 3 : ���n�̓d�g��M���x�A�C�R��
                                4 �` 7 : ���n�̓d�g��M���x�A�C�R��
                                8      : �ʐM�A�C�R��
                                9      : �s�N�g�`���b�g�A�C�R��
                x           - X���W�ʒu���h�b�g�P�ʂŎw��
                y           - Y���W�ʒu���h�b�g�P�ʂŎw��

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void DrawIcon(u8 index, int charName, int x, int y)
{
    G2_SetOBJAttr(&gOam[index],        // �ҏW���� OAM �\���̂ւ̃|�C���^
                  x,                   // X �ʒu
                  y,                   // Y �ʒu
                  0,                   // �\���v���C�I���e�B �ō�
                  GX_OAM_MODE_NORMAL,  // �m�[�}�� OBJ
                  FALSE,               // ���U�C�N�Ȃ�
                  GX_OAM_EFFECT_NONE,  // �G�t�F�N�g�Ȃ�
                  GX_OAM_SHAPE_16x16,  // 2 �~ 2 �L�����N�^
                  GX_OAM_COLORMODE_16, // 16�F�p���b�g
                  charName * 4,        // �L�����N�^�ԍ�
                  0,                   // �p���b�g�ԍ� 0
                  0                    // �A�t�B���ϊ��C���f�b�N�X(����)
        );
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
