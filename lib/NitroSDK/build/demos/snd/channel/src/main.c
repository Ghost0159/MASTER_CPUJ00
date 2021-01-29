/*---------------------------------------------------------------------------*
  Project:  NitroSDK - SND - demos - channel
  File:     main.c

  Copyright 2005,2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: main.c,v $
  Revision 1.3  2006/01/18 02:11:29  kitase_hirotake
  do-indent

  Revision 1.2  2005/11/21 11:02:42  kitase_hirotake
  SVC_WaitVBlankIntr �� OS_WaitVBlankIntr �ɕύX

  Revision 1.1  2005/04/13 03:29:29  ida
  SND��NITRO-SDK�ֈڐ�

  Revision 1.3  2005/04/13 01:15:26  kyuma_koichi
  �g�`�t�@�C���t�H�[�}�b�g�̕ύX

  Revision 1.2  2005/03/08 07:45:24  kyuma_koichi
  ����

  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#include <nitro.h>
#include "organ_det.g3.pcm16.h"
#include "wihaho.pcm16.h"

// PSG�̎��g�����v�Z����}�N��
#define FreqToTimer(freq) (SND_TIMER_CLOCK / ( 8 * (freq) ))
#define KeyToTimer(key) (SND_CalcTimer(FreqToTimer(440), ((key)-69)*64))

#define PCM_PLAY_CHANNEL1   4
#define PCM_PLAY_CHANNEL2   5
#define PSG_PLAY_CHANNEL    8
#define NOISE_PLAY_CHANNEL  14

#define CENTER_PAN 64

u16     Cont;
u16     Trg;
u8      key = 60;

void    VBlankIntr(void);

/*---------------------------------------------------------------------------*
  Name:         NitroMain

  Description:  main

  Arguments:    None

  Returns:      None
 *---------------------------------------------------------------------------*/
void NitroMain()
{
    // ������
    OS_Init();
    GX_Init();
    SND_Init();

    // V�u�����N���荞�ݐݒ�
    OS_SetIrqFunction(OS_IE_V_BLANK, VBlankIntr);
    (void)OS_EnableIrqMask(OS_IE_V_BLANK);
    (void)OS_EnableIrq();
    (void)GX_VBlankIntr(TRUE);

    // print usage
    OS_Printf("=================================\n");
    OS_Printf("USAGE:\n");
    OS_Printf(" A, B, X, Y : start sound\n");
    OS_Printf(" START : stop sound\n");
    OS_Printf("=================================\n");

    // �`�����l�������b�N����
    SND_LockChannel((1 << PCM_PLAY_CHANNEL1) | (1 << PCM_PLAY_CHANNEL2) | (1 << PSG_PLAY_CHANNEL) |
                    (1 << NOISE_PLAY_CHANNEL), 0);

    while (1)
    {
        u16     ReadData;

        OS_WaitVBlankIntr();

        // �`�q�l�V�R�}���h������M
        while (SND_RecvCommandReply(SND_COMMAND_NOBLOCK) != NULL)
        {
        }

        ReadData = PAD_Read();
        Trg = (u16)(ReadData & (ReadData ^ Cont));
        Cont = ReadData;

        // PCM�Đ�
        if (Trg & PAD_BUTTON_A)
        {
            SND_SetupChannelPcm(PCM_PLAY_CHANNEL1,
                                ORGAN_DET_G3_PCM16_FORMAT,
                                organ_det_g3_pcm16,
                                ORGAN_DET_G3_PCM16_LOOPFLAG ? SND_CHANNEL_LOOP_REPEAT :
                                SND_CHANNEL_LOOP_1SHOT, ORGAN_DET_G3_PCM16_LOOPSTART,
                                ORGAN_DET_G3_PCM16_LOOPLEN, 127, SND_CHANNEL_DATASHIFT_NONE,
                                SND_CalcTimer(ORGAN_DET_G3_PCM16_TIMER, ((key) - 67) * 64),
                                CENTER_PAN);
            SND_StartTimer(1 << PCM_PLAY_CHANNEL1, 0, 0, 0);
        }

        if (Trg & PAD_BUTTON_B)
        {
            SND_SetupChannelPcm(PCM_PLAY_CHANNEL2,
                                WIHAHO_PCM16_FORMAT,
                                wihaho_pcm16,
                                WIHAHO_PCM16_LOOPFLAG ? SND_CHANNEL_LOOP_REPEAT :
                                SND_CHANNEL_LOOP_1SHOT, WIHAHO_PCM16_LOOPSTART,
                                WIHAHO_PCM16_LOOPLEN, 127, SND_CHANNEL_DATASHIFT_NONE,
                                WIHAHO_PCM16_TIMER, CENTER_PAN);
            SND_StartTimer(1 << PCM_PLAY_CHANNEL2, 0, 0, 0);
        }

        // PSG��`�g���Đ�
        if (Trg & PAD_BUTTON_X)
        {
            SND_SetupChannelPsg(PSG_PLAY_CHANNEL,
                                SND_DUTY_4_8,
                                64, SND_CHANNEL_DATASHIFT_NONE, KeyToTimer(key), CENTER_PAN);
            SND_StartTimer(1 << PSG_PLAY_CHANNEL, 0, 0, 0);
        }

        // �m�C�Y�̍Đ�
        if (Trg & PAD_BUTTON_Y)
        {
            SND_SetupChannelNoise(NOISE_PLAY_CHANNEL,
                                  64, SND_CHANNEL_DATASHIFT_NONE, KeyToTimer(key), CENTER_PAN);
            SND_StartTimer(1 << NOISE_PLAY_CHANNEL, 0, 0, 0);
        }

        // ��~
        if (Trg & PAD_BUTTON_START)
        {
            SND_StopTimer((1 << PCM_PLAY_CHANNEL1) | (1 << PCM_PLAY_CHANNEL2) |
                          (1 << PSG_PLAY_CHANNEL) | (1 << NOISE_PLAY_CHANNEL), 0, 0, 0);
        }

        // �Đ��̃L�[�𔼉��グ��
        if (Trg & PAD_KEY_UP)
        {
            key++;
            SND_SetChannelTimer(1 << PCM_PLAY_CHANNEL1,
                                SND_CalcTimer(ORGAN_DET_G3_PCM16_TIMER, ((key) - 67) * 64));
            SND_SetChannelTimer((1 << PSG_PLAY_CHANNEL) | (1 << NOISE_PLAY_CHANNEL),
                                KeyToTimer(key));
            OS_Printf("key: %d\n", key);
        }

        // �Đ��̃L�[�𔼉�������
        if (Trg & PAD_KEY_DOWN)
        {
            key--;
            SND_SetChannelTimer(1 << PCM_PLAY_CHANNEL1,
                                SND_CalcTimer(ORGAN_DET_G3_PCM16_TIMER, ((key) - 67) * 64));
            SND_SetChannelTimer((1 << PSG_PLAY_CHANNEL) | (1 << NOISE_PLAY_CHANNEL),
                                KeyToTimer(key));
            OS_Printf("key: %d\n", key);
        }

        // �R�}���h�t���b�V��
        (void)SND_FlushCommand(SND_COMMAND_NOBLOCK);
    }
}

//--------------------------------------------------------------------------------
//    �u�u�����N���荞�ݏ���
//
void VBlankIntr(void)
{
    OS_SetIrqCheckFlag(OS_IE_V_BLANK); // checking VBlank interrupt
}
