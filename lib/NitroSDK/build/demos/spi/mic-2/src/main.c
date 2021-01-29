/*---------------------------------------------------------------------------*
  Project:  NitroSDK - SPI - demos - mic-2
  File:     main.c

  Copyright 2003-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: main.c,v $
  Revision 1.14  2007/05/14 06:39:09  yosizaki
  add option for tool.

  Revision 1.13  2006/01/18 02:11:29  kitase_hirotake
  do-indent

  Revision 1.12  2005/11/21 11:03:53  kitase_hirotake
  OS_InitPrintServer , OS_PrintServer , PXI_Init �̍폜
  SVC_WaitVBlankIntr �� OS_WaitVBlankIntr �ɕύX

  Revision 1.11  2005/04/08 01:49:02  yosizaki
  add DC_InvalidateRange before reading the caputure-buffer.

  Revision 1.10  2005/02/28 05:26:23  yosizaki
  do-indent.

  Revision 1.9  2004/11/04 07:18:57  terui
  MIC_GetLastSamplingAddress�̎d�l�C���ɔ������C���B

  Revision 1.8  2004/09/17 09:36:03  terui
  MIC_StartAutoSampling(Async)�̃p�����[�^�ύX�ɔ����C���B

  Revision 1.7  2004/08/23 07:27:27  yasu
  Workaround for pushing A button in IPL

  Revision 1.6  2004/08/20 02:44:31  terui
  TS_VERSION=0���ɂ�PM_SetAmp����悤�C���B

  Revision 1.5  2004/08/05 07:38:59  takano_makoto
  �񋓌^�ύXMICSamplingType�̕ύX�ɔ����C��

  Revision 1.4  2004/08/04 23:44:28  terui
  LCD�o�b�N���C�g����֐��̕ύX

  Revision 1.3  2004/08/02 00:47:45  yada
  PM_GAINAMP_80 -> PM_AMPGAIN_80

  Revision 1.2  2004/07/31 12:46:19  terui
  PMIC���}�C�N�p�ɏ���������R�[�h��ǉ�

  Revision 1.1  2004/06/17 04:42:46  yosizaki
  (none)

  $NoKeywords: $
 *---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*
    A sample that controls mic sampling status.

    USAGE:
        UP , DOWN    : Control sampling span.
        LEFT , RIGHT : Control sampling bit range. ( 8bit or 12 bit )
        A            : start / stop sampling. (toggle)
        B            : force-stop. (stop and ignore rest data)

    HOWTO:
        1. Initialize memory allocate system to get 32byte aligned big buffer.
        2. Initialize MIC library.
        3. When sampling is stopped, you can change status
           and start auto sampling.
           Debug-output is sampling data for tool "mic2wav.exe".
        4. Debug-output log can make its waveform files by tool "mic2wav.exe".
           > $(NITROSDK_ROOT)/tools/bin/mic2wav [logfile] [,directory]
           each sampling data (separated by A-Button) creates a waveform file
           in "[directory]/%08X.wav".

    NOTE:
        1. The speed of debug-output is later than sampling.
           When you stop sampling, then please wait all data is printed.

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


/* mic-1 �Ƃ̈Ⴂ��, �f�[�^���f�o�b�O�o�͂��邱�Ƃ�
   ��Ƀ����V���b�g�^�����邱�Ƃł��B */

/* �g�`�o�͗p�ϐ� */
static volatile BOOL g_sample_busy = FALSE;
static const void *g_record_smps = NULL;

static void StartSampling(void);
static void OnSampleDone(MICResult result, void *arg);
static void OutputSampleWave(void *dat, MICSamplingType type);
static void StopSamplingOutput(void);

/* �T���v�����O���J�n */
static void StartSampling(void)
{
    gMicAutoParam.buffer = (void *)gMicData;
    gMicAutoParam.size = TEST_BUFFER_SIZE;
    gMicAutoParam.loop_enable = FALSE;
    gMicAutoParam.full_callback = OnSampleDone;
    g_sample_busy = TRUE;
    g_record_smps = gMicData;
    PrintfVariableData();
    (void)MIC_StartAutoSampling(&gMicAutoParam);
}

/* �T���v�����O�o�͂��I�� */
static void StopSamplingOutput(void)
{
    OS_PutString("$end\n");
    OS_PutString("\n");
    g_record_smps = NULL;
}

/* �T���v�����O�����ʒm, �܂��͒�~���̔g�`�o�͏��� */
static void OnSampleDone(MICResult result, void *arg)
{
    (void)result;
    (void)arg;
    if (g_sample_busy)
    {
        g_sample_busy = FALSE;
    }
}

/* �T���v�����O���̔g�`�����O�o�� */
static void OutputSampleWave(void *dat, MICSamplingType type)
{
    /* 16 �T���v���P�ʂ� 1 ��� 2 �s�܂�.
       ����ȏ�̗ʂ��o�͂���ƁA���O���������邱�Ƃ�����. */
    enum
    { smps_per_line = 16, max_line_per_frame = 2 };
    if (!g_record_smps || !dat)
        return;

    DC_InvalidateRange((void *)g_record_smps, (u32)((u8 *)dat - (u8 *)g_record_smps));
    switch (type)
    {
    case MIC_SAMPLING_TYPE_8BIT:
        {
            typedef u8 SMP;
            /* �ȉ��� MIC_SAMPLING_TYPE_12BIT �Ɠ��� */
            char    buf[1 + (sizeof(SMP) * 2 + 1) * smps_per_line + 1 + 1], *s;
            const SMP *from = (const SMP *)g_record_smps;
            const SMP *to = (const SMP *)dat;
            int     lines = 0;
            while ((lines < max_line_per_frame) && (from + smps_per_line <= to))
            {
                int     i, j;
                s = buf;
                *s++ = '|';
                for (i = 0; i < smps_per_line; ++i)
                {
                    u32     unit = from[i];
                    for (j = sizeof(SMP) * 8; (j -= 4) >= 0;)
                    {
                        u32     c = (u32)((unit >> j) & 0x0F);
                        c += (u32)((c < 10) ? ('0' - 0) : ('A' - 10));
                        MI_WriteByte(s++, (u8)c);
                    }
                    MI_WriteByte(s++, (u8)',');
                }
                MI_WriteByte(s++, (u8)'\n');
                MI_WriteByte(s++, (u8)'\0');
                OS_PutString(buf);
                from += smps_per_line;
                ++lines;
            }
            g_record_smps = from;

        }
        break;
    case MIC_SAMPLING_TYPE_12BIT:
        {
            typedef u16 SMP;
            /* �ȉ��� MIC_SAMPLING_TYPE_8BIT �Ɠ��� */
            char    buf[1 + (sizeof(SMP) * 2 + 1) * smps_per_line + 1 + 1], *s;
            const SMP *from = (const SMP *)g_record_smps;
            const SMP *to = (const SMP *)dat;
            int     lines = 0;
            while ((lines < max_line_per_frame) && (from + smps_per_line <= to))
            {
                int     i, j;
                s = buf;
                *s++ = '|';
                for (i = 0; i < smps_per_line; ++i)
                {
                    u32     unit = from[i];
                    for (j = sizeof(SMP) * 8; (j -= 4) >= 0;)
                    {
                        u32     c = (u32)((unit >> j) & 0x0F);
                        c += (u32)((c < 10) ? ('0' - 0) : ('A' - 10));
                        MI_WriteByte(s++, (u8)c);
                    }
                    MI_WriteByte(s++, (u8)',');
                }
                MI_WriteByte(s++, (u8)'\n');
                MI_WriteByte(s++, (u8)'\0');
                OS_PutString(buf);
                from += smps_per_line;
                ++lines;
            }
            g_record_smps = from;

        }
        break;
    }

    /* �T���v�����O��~����c��̃f�[�^���o��. */
    if (!g_sample_busy && g_record_smps)
    {
        if ((u8 *)g_record_smps + smps_per_line * 2 >= (u8 *)dat)
        {
            StopSamplingOutput();
        }
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
    gMicAutoParam.type = MIC_SAMPLING_TYPE_12BIT;
    gMicAutoParam.rate = MIC_SAMPLING_RATE_11K;
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

    // �J�n���b�Z�[�W�o��
    OS_Printf("#ARM9: MIC demo started.\n");
    OS_Printf("#   up/down    -> change sampling span\n");
    OS_Printf("#   left/right -> change bit range\n");
    OS_Printf("#   A          -> start / stop\n");
    OS_Printf("#   B          -> force-stop\n");
    OS_Printf("#   select     -> terminate\n");

    // �L�[���͏��擾�̋�Ăяo��(IPL �ł� A �{�^�������΍�)
    KeyRead(&gKey);

    // main loop
    while (TRUE)
    {
        // �L�[���͏��擾
        KeyRead(&gKey);

        // A �������ꂽ��J�n, �ēx A �������ꂽ���~.
        if ((gKey.trg & PAD_BUTTON_A) != 0)
        {
            if (!g_sample_busy)
            {
                StartSampling();
            }
            else
            {
                (void)MIC_StopAutoSampling();
                OnSampleDone(MIC_RESULT_SUCCESS, &gMicAutoParam);
            }
        }
        // B �������ꂽ��, ��~���Ďc��̃f�[�^������.
        if ((gKey.trg & PAD_BUTTON_B) != 0)
        {
            if (g_sample_busy)
            {
                (void)MIC_StopAutoSampling();
                OnSampleDone(MIC_RESULT_SUCCESS, &gMicAutoParam);
            }
            if (g_record_smps)
            {
                StopSamplingOutput();
            }
        }
        // SELECT �������ꂽ��v���O�����������I��.
        if ((gKey.trg & PAD_BUTTON_SELECT) != 0)
        {
            OS_Exit(0);
        }

        // �T���v�����O�o�͒��łȂ���Ήσp�����[�^�ύX
        if (!g_record_smps)
        {
            // �T���v�����O���(�r�b�g��)�ύX
            if ((gKey.trg | gKey.rep) & (PAD_KEY_LEFT | PAD_KEY_RIGHT))
            {
                //****************************************************************
                if (gMicAutoParam.type == MIC_SAMPLING_TYPE_8BIT)
                {
                    gMicAutoParam.type = MIC_SAMPLING_TYPE_12BIT;
                }
                else
                {
                    gMicAutoParam.type = MIC_SAMPLING_TYPE_8BIT;
                }
                //****************************************************************
                if (!g_record_smps)
                    PrintfVariableData();
            }
            // �T���v�����O���[�g�ύX
            if ((gKey.trg | gKey.rep) & PAD_KEY_UP)
            {
                //****************************************************************
                switch (gMicAutoParam.rate)
                {
                case MIC_SAMPLING_RATE_8K:
                    gMicAutoParam.rate = MIC_SAMPLING_RATE_11K;
                    break;
                case MIC_SAMPLING_RATE_11K:
                    gMicAutoParam.rate = MIC_SAMPLING_RATE_16K;
                    break;
                case MIC_SAMPLING_RATE_16K:
                    gMicAutoParam.rate = MIC_SAMPLING_RATE_22K;
                    break;
                case MIC_SAMPLING_RATE_22K:
                    gMicAutoParam.rate = MIC_SAMPLING_RATE_32K;
                    break;
                case MIC_SAMPLING_RATE_32K:
                    gMicAutoParam.rate = MIC_SAMPLING_RATE_8K;
                    break;
                }
                //****************************************************************
                if (!g_record_smps)
                    PrintfVariableData();
            }
            if ((gKey.trg | gKey.rep) & PAD_KEY_DOWN)
            {
                //****************************************************************
                switch (gMicAutoParam.rate)
                {
                case MIC_SAMPLING_RATE_8K:
                    gMicAutoParam.rate = MIC_SAMPLING_RATE_32K;
                    break;
                case MIC_SAMPLING_RATE_11K:
                    gMicAutoParam.rate = MIC_SAMPLING_RATE_8K;
                    break;
                case MIC_SAMPLING_RATE_16K:
                    gMicAutoParam.rate = MIC_SAMPLING_RATE_11K;
                    break;
                case MIC_SAMPLING_RATE_22K:
                    gMicAutoParam.rate = MIC_SAMPLING_RATE_16K;
                    break;
                case MIC_SAMPLING_RATE_32K:
                    gMicAutoParam.rate = MIC_SAMPLING_RATE_22K;
                    break;
                }
                //****************************************************************
                if (!g_record_smps)
                    PrintfVariableData();
            }
        }

        // �g�`�����O�o��.
        OutputSampleWave(MIC_GetLastSamplingAddress(), gMicAutoParam.type);

        // �g�`�`��
        if (g_sample_busy)
        {
            SetDrawData(MIC_GetLastSamplingAddress(), gMicAutoParam.type);
            Draw3D();
        }

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
  Name:         Init3D

  Description:  3D�ɂĕ\�����邽�߂̏���������

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void Init3D(void)
{
    G3X_Init();
    G3X_InitMtxStack();
    G3_SwapBuffers(GX_SORTMODE_AUTO, GX_BUFFERMODE_W);
    GX_SetGraphicsMode(GX_DISPMODE_GRAPHICS, GX_BGMODE_0, GX_BG0_AS_3D);
    GX_SetVisiblePlane(GX_PLANEMASK_BG0);
    G2_SetBG0Priority(0);
    G3X_SetShading(GX_SHADING_TOON);
    G3X_AlphaTest(FALSE, 0);
    G3X_AlphaBlend(TRUE);
    G3X_AntiAlias(TRUE);
    G3X_EdgeMarking(FALSE);
    G3X_SetFog(FALSE, (GXFogBlend)0, (GXFogSlope)0, 0);
    G3X_SetClearColor(0, 0, 0x7fff, 63, FALSE);
    G3_ViewPort(0, 0, 255, 191);
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

    G3_MtxMode(GX_MTXMODE_PROJECTION);
    G3_Identity();
    G3_MtxMode(GX_MTXMODE_POSITION_VECTOR);
    G3_Identity();

    G3_PolygonAttr(GX_LIGHTMASK_NONE, GX_POLYGONMODE_MODULATE, GX_CULL_NONE, 0, 31, 0);

    if (g_sample_busy)
    {
        s32     i;

        for (i = 0; i < 191; i++)
        {
            DrawLine((s16)(gDrawData[i]), (s16)i, (s16)(gDrawData[i + 1]), (s16)(i + 1));
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
    fx16    fsx, fsy, fex, fey;

    fsx = (fx16)(((sx - 128) * 0x1000) / 128);
    fsy = (fx16)(((96 - sy) * 0x1000) / 96);
    fex = (fx16)(((ex - 128) * 0x1000) / 128);
    fey = (fx16)(((96 - ey) * 0x1000) / 96);

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

    if (!address)
        return;

    if (type == MIC_SAMPLING_TYPE_8BIT)
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

    if (type == MIC_SAMPLING_TYPE_12BIT)
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
    s32     rate = 0;

    switch (gMicAutoParam.type)
    {
    case MIC_SAMPLING_TYPE_8BIT:
        range = 8;
        break;
    case MIC_SAMPLING_TYPE_12BIT:
        range = 16;
        break;
    }

    switch (gMicAutoParam.rate)
    {
    case MIC_SAMPLING_RATE_8K:
        rate = 8000;
        break;
    case MIC_SAMPLING_RATE_11K:
        rate = 11025;
        break;
    case MIC_SAMPLING_RATE_16K:
        rate = 16000;
        break;
    case MIC_SAMPLING_RATE_22K:
        rate = 22050;
        break;
    case MIC_SAMPLING_RATE_32K:
        rate = 32000;
        break;
    }

    OS_Printf("$rate=%d\n$bits=%d\n", rate, range);
}

/*---------------------------------------------------------------------------*
  End of file
 *---------------------------------------------------------------------------*/
