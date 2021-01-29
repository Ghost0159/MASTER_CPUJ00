/*---------------------------------------------------------------------------*
  Project:  NitroSDK - SPI - demos
  File:     main.c

  Copyright 2003-2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: main.c,v $
  Revision 1.24  2006/01/18 02:11:29  kitase_hirotake
  do-indent

  Revision 1.23  2005/11/21 11:03:42  kitase_hirotake
  OS_InitPrintServer , OS_PrintServer , PXI_Init �̍폜
  SVC_WaitVBlankIntr �� OS_WaitVBlankIntr �ɕύX

  Revision 1.22  2005/02/28 05:26:23  yosizaki
  do-indent.

  Revision 1.21  2005/01/11 07:51:03  takano_makoto
  fix copyright header.

  Revision 1.20  2005/01/11 07:48:58  terui
  Update copyright.

  Revision 1.19  2005/01/07 12:34:58  terui
  �T���v�����O���[�g�ύX����MIC_AdjustAutoSampling�֐���p����悤�ɕύX�B

  Revision 1.18  2004/12/03 05:40:03  terui
  MIC�̎����T���v�����O���ɔ������f�[�^�𖾎��I�Ɏ����@�\�ǉ��ɑΉ��B

  Revision 1.17  2004/11/04 07:18:57  terui
  MIC_GetLastSamplingAddress�̎d�l�C���ɔ������C���B

  Revision 1.16  2004/09/17 09:36:03  terui
  MIC_StartAutoSampling(Async)�̃p�����[�^�ύX�ɔ����C���B

  Revision 1.15  2004/09/06 12:59:44  terui
  Only fix indent.

  Revision 1.14  2004/08/23 07:27:27  yasu
  Workaround for pushing A button in IPL

  Revision 1.13  2004/08/20 02:44:31  terui
  TS_VERSION=0���ɂ�PM_SetAmp����悤�C���B

  Revision 1.12  2004/08/04 23:44:28  terui
  LCD�o�b�N���C�g����֐��̕ύX

  Revision 1.11  2004/08/02 07:33:45  terui
  Only fix comment

  Revision 1.10  2004/08/02 00:42:01  yada
  PM_GAINAMP_80 -> PM_AMPGAIN_80

  Revision 1.9  2004/07/31 12:46:19  terui
  PMIC���}�C�N�p�ɏ���������R�[�h��ǉ�

  Revision 1.8  2004/07/13 09:18:52  terui
  �����t���̃T���v�����O��ʒǉ��ɔ����ύX

  Revision 1.7  2004/06/10 06:51:37  terui
  �Ӑ}���Ă��Ȃ��A�h���X�̃L���b�V����j������\�����������_���C���B

  Revision 1.6  2004/06/03 04:09:50  terui
  �ŒZ�T���v�����O���[�g��`��ύX

  Revision 1.5  2004/06/02 06:14:15  terui
  12�r�b�g�T���v�����O����0�p�f�B���O�����ʂɕύX���ꂽ���ƂɑΉ��B

  Revision 1.4  2004/06/01 02:05:00  terui
  �����T���v�����O���[�g��MIC_SAMPLING_RATE_8K�ɕύX�B

  Revision 1.3  2004/06/01 00:58:24  terui
  �T���v�����O������ARM7�̃N���b�N�P�ʂŒ��ڎw�肷��悤�ύX�B

  Revision 1.2  2004/05/31 09:14:38  terui
  Add explanation of this demo.

  Revision 1.1  2004/05/31 08:58:51  terui
  Initial upload.

  $NoKeywords: $
 *---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*
    A sample that controls mic sampling status.

    USAGE:
        UP , DOWN    : Control sampling span.
        LEFT , RIGHT : Control sampling bit range. ( 8bit or 12 bit etc. )
        SEL , STA    : Control loop sampling enable or disable.

    HOWTO:
        1. Initialize memory allocate system to get 32byte aligned big buffer.
        2. Initialize MIC library.
        3. Start auto sampling of MIC by default status.
        4. When you change status , first , stop auto sampling.
           Then , edit status and start auto sampling again.
 *---------------------------------------------------------------------------*/

#include    <nitro.h>

#ifdef  SDK_TS
#if     ( SDK_TS_VERSION >= 100 )
#include    <nitro/spi/common/pm_common.h>
#include    <nitro/spi/ARM9/pm.h>
#endif
#endif


/*---------------------------------------------------------------------------*
    �萔��`
 *---------------------------------------------------------------------------*/
#define     KEY_REPEAT_START    25     // �L�[���s�[�g�J�n�܂ł̃t���[����
#define     KEY_REPEAT_SPAN     10     // �L�[���s�[�g�̊Ԋu�t���[����
#define     TEST_BUFFER_SIZE    ( 1024 * 1024 ) // 1M


/*---------------------------------------------------------------------------*
    �\���̒�`
 *---------------------------------------------------------------------------*/
// �L�[���͏��
typedef struct KeyInformation
{
    u16     cnt;                       // �����H���͒l
    u16     trg;                       // �����g���K����
    u16     up;                        // �����g���K����
    u16     rep;                       // �����ێ����s�[�g����

}
KeyInformation;

/*---------------------------------------------------------------------------*
    �����֐���`
 *---------------------------------------------------------------------------*/
static void InitializeAllocateSystem(void);
static void Init3D(void);
static void Draw3D(void);
static void DrawLine(s16 sx, s16 sy, s16 ex, s16 ey);
static void VBlankIntr(void);
static void KeyRead(KeyInformation * pKey);
static void SetDrawData(void *address, MICSamplingType type);
static void PrintfVariableData(void);

/*---------------------------------------------------------------------------*
    �����ϐ���`
 *---------------------------------------------------------------------------*/
static KeyInformation gKey;
static MICAutoParam gMicAutoParam;
static u8 *gMicData;
static u8 gDrawData[192];


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

    // 3D�֘A������
    Init3D();

    // �����ݐݒ�
    OS_SetIrqFunction(OS_IE_V_BLANK, VBlankIntr);
    (void)OS_EnableIrqMask(OS_IE_V_BLANK);
    (void)OS_EnableIrq();
    (void)OS_EnableInterrupts();
    (void)GX_VBlankIntr(TRUE);

    //****************************************************************
    // Initialize MIC.
    InitializeAllocateSystem();
    // OS_Alloc�Ŋm�ۂ�����������32�o�C�gALIGN�ɂȂ��Ă���̂ŁA
    // �L���b�V�����삵�Ă����̃��������󂳂Ȃ��B
    gMicData = (u8 *)OS_Alloc(TEST_BUFFER_SIZE);
    MIC_Init();

#ifdef  SDK_TS
    // PMIC������
    PM_Init();
    // AMP�I��
    (void)PM_SetAmp(PM_AMP_ON);
#if ( SDK_TS_VERSION >= 100 )
    // AMP�Q�C������
    (void)PM_SetAmpGain(PM_AMPGAIN_80);
#endif
#if ( SDK_TS_VERSION == 100 )
    // �m�C�Y�΍�ׁ̈ALCD�o�b�N���C�g�I�t
    (void)PM_SetBackLight(PM_LCD_ALL, PM_BACKLIGHT_OFF);
#endif
#endif

    gMicAutoParam.type = MIC_SAMPLING_TYPE_8BIT;
    gMicAutoParam.buffer = (void *)gMicData;
    gMicAutoParam.size = TEST_BUFFER_SIZE;
    gMicAutoParam.rate = MIC_SAMPLING_RATE_8K;
    gMicAutoParam.loop_enable = TRUE;
    gMicAutoParam.full_callback = NULL;
    (void)MIC_StartAutoSampling(&gMicAutoParam);
    //****************************************************************

    // �����ϐ�������
    {
        s32     i;

        for (i = 0; i < 192; i++)
        {
            gDrawData[i] = 0x80;
        }
    }

    // LCD�\���J�n
    GX_DispOn();
    GXS_DispOn();

    // �f�o�b�O������o��
    OS_Printf("ARM9: MIC demo started.\n");
    OS_Printf("   up/down    -> change sampling span\n");
    OS_Printf("   left/right -> change bit range\n");
    OS_Printf("   select     -> change loop setting\n");
    OS_Printf("\n");
    PrintfVariableData();

    // �L�[���͏��擾�̋�Ăяo��(IPL �ł� A �{�^�������΍�)
    KeyRead(&gKey);

    // main loop
    while (TRUE)
    {
        // �L�[���͏��擾
        KeyRead(&gKey);

        // �σp�����[�^�ύX
        {
            // �T���v�����O���(�r�b�g��)�ύX
            if ((gKey.trg | gKey.rep) & (PAD_KEY_LEFT | PAD_KEY_RIGHT))
            {
                //****************************************************************
                (void)MIC_StopAutoSampling();
                gMicAutoParam.type = (MICSamplingType)((gMicAutoParam.type +
                                                        1) % MIC_SAMPLING_TYPE_MAX);
                (void)MIC_StartAutoSampling(&gMicAutoParam);
                //****************************************************************
                PrintfVariableData();
            }
            // �T���v�����O���[�g�ύX
            if ((gKey.trg | gKey.rep) & PAD_KEY_UP)
            {
                //****************************************************************
                gMicAutoParam.rate = (u32)(gMicAutoParam.rate / 2);
                if (gMicAutoParam.rate < MIC_SAMPLING_RATE_LIMIT)
                {
                    gMicAutoParam.rate = MIC_SAMPLING_RATE_LIMIT;
                }
                (void)MIC_AdjustAutoSampling(gMicAutoParam.rate);
                //****************************************************************
                PrintfVariableData();
            }
            if ((gKey.trg | gKey.rep) & PAD_KEY_DOWN)
            {
                //****************************************************************
                gMicAutoParam.rate = (u32)(gMicAutoParam.rate * 2);
                if (gMicAutoParam.rate < MIC_SAMPLING_RATE_LIMIT)
                {
                    gMicAutoParam.rate = MIC_SAMPLING_RATE_LIMIT;
                }
                (void)MIC_AdjustAutoSampling(gMicAutoParam.rate);
                //****************************************************************
                PrintfVariableData();
            }
            // �o�b�t�@�t�����̃��[�v�ەύX
            if ((gKey.trg | gKey.rep) & (PAD_BUTTON_SELECT | PAD_BUTTON_START))
            {
                //****************************************************************
                (void)MIC_StopAutoSampling();
                gMicAutoParam.loop_enable = (gMicAutoParam.loop_enable + 1) % 2;
                (void)MIC_StartAutoSampling(&gMicAutoParam);
                //****************************************************************
                PrintfVariableData();
            }
        }

        // �g�`�`��
        SetDrawData(MIC_GetLastSamplingAddress(), gMicAutoParam.type);
        Draw3D();

        // �u�u�����N�҂�
        OS_WaitVBlankIntr();
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
  Name:         Init3D

  Description:  3D�ɂĕ\�����邽�߂̏���������

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void Init3D(void)
{
    G3X_Init();
    G3X_InitMtxStack();
    GX_SetGraphicsMode(GX_DISPMODE_GRAPHICS, GX_BGMODE_0, GX_BG0_AS_3D);
    GX_SetVisiblePlane(GX_PLANEMASK_BG0);
    G2_SetBG0Priority(0);
    G3X_AlphaTest(FALSE, 0);
    G3X_AntiAlias(TRUE);
    G3X_EdgeMarking(FALSE);
    G3X_SetFog(FALSE, (GXFogBlend)0, (GXFogSlope)0, 0);
    G3X_SetClearColor(0, 0, 0x7fff, 63, FALSE);
    G3_ViewPort(0, 0, 255, 191);
    G3_MtxMode(GX_MTXMODE_POSITION_VECTOR);
}

/*---------------------------------------------------------------------------*
  Name:         Draw3D

  Description:  3D�\���ɂĔg�`��\��

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void Draw3D(void)
{
    G3X_Reset();
    G3_Identity();
    G3_PolygonAttr(GX_LIGHTMASK_NONE, GX_POLYGONMODE_MODULATE, GX_CULL_NONE, 0, 31, 0);

    {
        s32     i;

        if ((gMicAutoParam.type == MIC_SAMPLING_TYPE_SIGNED_8BIT) ||
            (gMicAutoParam.type == MIC_SAMPLING_TYPE_SIGNED_12BIT) ||
            (gMicAutoParam.type == MIC_SAMPLING_TYPE_SIGNED_12BIT_FILTER_OFF))
        {
            for (i = 0; i < 191; i++)
            {
                DrawLine((s16)((s8)gDrawData[i]),
                         (s16)i, (s16)((s8)gDrawData[i + 1]), (s16)(i + 1));
            }
        }
        else
        {
            for (i = 0; i < 191; i++)
            {
                DrawLine((s16)(gDrawData[i]), (s16)i, (s16)(gDrawData[i + 1]), (s16)(i + 1));
            }
        }
    }

    G3_SwapBuffers(GX_SORTMODE_AUTO, GX_BUFFERMODE_W);
}

/*---------------------------------------------------------------------------*
  Name:         DrawLine

  Description:  �O�p�|���S���Ő���`��

  Arguments:    sx  - �`�悷����̊J�n�_�̂����W
                sy  - �`�悷����̊J�n�_�̂����W
                ex  - �`�悷����̏I�_�̂����W
                ey  - �`�悷����̏I�_�̂����W

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void DrawLine(s16 sx, s16 sy, s16 ex, s16 ey)
{
    fx16    fsx = (fx16)(((sx - 128) * 0x1000) / 128);
    fx16    fsy = (fx16)(((96 - sy) * 0x1000) / 96);
    fx16    fex = (fx16)(((ex - 128) * 0x1000) / 128);
    fx16    fey = (fx16)(((96 - ey) * 0x1000) / 96);

    G3_Begin(GX_BEGIN_TRIANGLES);
    {
        G3_Color(GX_RGB(31, 31, 31));
        G3_Vtx(fsx, fsy, 0);
        G3_Color(GX_RGB(31, 31, 31));
        G3_Vtx(fex, fey, 0);
        G3_Color(GX_RGB(31, 31, 31));
        G3_Vtx(fsx, fsy, 1);
    }
    G3_End();
}

/*---------------------------------------------------------------------------*
  Name:         VBlankIntr

  Description:  �u�u�����N�����݃x�N�g���B

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void VBlankIntr(void)
{
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
static void KeyRead(KeyInformation * pKey)
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
  Name:         SetDrawData

  Description:  ����ł̍ŐV�T���v�����O�ς݃f�[�^��\���ɔ��f������o�b�t�@��
                �i�[����B

  Arguments:    address - �R���|�[�l���g�ɂ���čŐV�̃T���v�����O�f�[�^��
                          �i�[���ꂽ���C����������̈ʒu�B
                type    - �T���v�����O���(�r�b�g��)

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void SetDrawData(void *address, MICSamplingType type)
{
    s32     i;

    // �T���v�����O�������s���Ă��Ȃ��ꍇ�͉��������ɏI��
    // (�}�C�N�Ɩ��֌W�̃������̃L���b�V����j�����Ă��܂���)
    if ((address < gMicData) || (address >= (gMicData + TEST_BUFFER_SIZE)))
    {
        return;
    }

    // 8bit�T���v�����O�̏ꍇ
    switch (type)
    {
    case MIC_SAMPLING_TYPE_8BIT:
    case MIC_SAMPLING_TYPE_SIGNED_8BIT:
        {
            u8     *p;

            p = (u8 *)((u32)address - 191);
            if (p < gMicData)
            {
                p = (u8 *)((u32)p + TEST_BUFFER_SIZE);
            }
            DC_InvalidateRange((void *)((u32)p & 0xffffffe0), 32);
            for (i = 0; i < 192; i++)
            {
                gDrawData[i] = *p;
                p++;
                if ((u32)p >= (u32)(gMicData + TEST_BUFFER_SIZE))
                {
                    p -= TEST_BUFFER_SIZE;
                }
                if (((u32)p % 32) == 0)
                {
                    DC_InvalidateRange(p, 32);
                }
            }
        }
        break;
    case MIC_SAMPLING_TYPE_12BIT:
    case MIC_SAMPLING_TYPE_SIGNED_12BIT:
    case MIC_SAMPLING_TYPE_12BIT_FILTER_OFF:
    case MIC_SAMPLING_TYPE_SIGNED_12BIT_FILTER_OFF:
        {
            u16    *p;

            p = (u16 *)((u32)address - 382);
            if ((u32)p < (u32)gMicData)
            {
                p = (u16 *)((u32)p + TEST_BUFFER_SIZE);
            }
            DC_InvalidateRange((void *)((u32)p & 0xffffffe0), 32);
            for (i = 0; i < 192; i++)
            {
                gDrawData[i] = (u8)((*p >> 8) & 0x00ff);
                p++;
                if ((u32)p >= (u32)(gMicData + TEST_BUFFER_SIZE))
                {
                    p = (u16 *)((u32)p - TEST_BUFFER_SIZE);
                }
                if (((u32)p % 32) == 0)
                {
                    DC_InvalidateRange(p, 32);
                }
            }
        }
        break;
    }
}

/*---------------------------------------------------------------------------*
  Name:         PrintfVariableData

  Description:  �ςȃT���v�����O�ݒ���v�����g�o�͂���B

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void PrintfVariableData(void)
{
    s32     range = 0;

    OS_Printf(" sampling-span: %d , bit-range: ", gMicAutoParam.rate);
    switch (gMicAutoParam.type)
    {
    case MIC_SAMPLING_TYPE_8BIT:
        OS_Printf("8");
        break;
    case MIC_SAMPLING_TYPE_12BIT:
        OS_Printf("12");
        break;
    case MIC_SAMPLING_TYPE_SIGNED_8BIT:
        OS_Printf("signed 8");
        break;
    case MIC_SAMPLING_TYPE_SIGNED_12BIT:
        OS_Printf("signed 12");
        break;
    case MIC_SAMPLING_TYPE_12BIT_FILTER_OFF:
        OS_Printf("12(filter off)");
        break;
    case MIC_SAMPLING_TYPE_SIGNED_12BIT_FILTER_OFF:
        OS_Printf("signed 12(filter off)");
        break;
    }
    if (gMicAutoParam.loop_enable)
    {
        OS_Printf(" , loop: on\n");
    }
    else
    {
        OS_Printf(" , loop: off\n");
    }
}


/*---------------------------------------------------------------------------*
  End of file
 *---------------------------------------------------------------------------*/
