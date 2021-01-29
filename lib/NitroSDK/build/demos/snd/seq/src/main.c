/*---------------------------------------------------------------------------*
  Project:  NitroSDK - SND - demos - seq
  File:     main.c

  Copyright 2005,2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: main.c,v $
  Revision 1.3  2006/01/18 02:11:19  kitase_hirotake
  do-indent

  Revision 1.2  2005/11/21 11:02:56  kitase_hirotake
  SVC_WaitVBlankIntr �� OS_WaitVBlankIntr �ɕύX

  Revision 1.1  2005/04/13 03:29:29  ida
  SND��NITRO-SDK�ֈڐ�

  Revision 1.3  2005/03/08 08:32:59  kyuma_koichi
  Warning�Ή�

  Revision 1.2  2005/03/08 07:45:24  kyuma_koichi
  ����

  $NoKeywords: $
 *---------------------------------------------------------------------------*/

//---------------------------------------------------------------------------
// USAGE:
//  A : start sequence
//  B : stop sequence
//---------------------------------------------------------------------------

#include <nitro.h>
#include "seq.h"
#include "channel.h"

#define MIDI_BUF_SIZE 8*1024
#define CHANNEL_NUM 4
#define ORIGINAL_KEY 67

u16     Cont;
u16     Trg;

u8      midifile_buf[MIDI_BUF_SIZE];

static SeqHandle handle;

void    VBlankIntr(void);
void    FileLoad(void *buf, const char *filename);
void    MidiCallback(const u8 *midi_event);


/*---------------------------------------------------------------------------*
  Name:         NitroMain

  Description:  ���C���֐�

  Arguments:    None

  Returns:      None
 *---------------------------------------------------------------------------*/
void NitroMain()
{
    // ������
    OS_Init();
    GX_Init();
    FS_Init(MI_DMA_MAX_NUM);
    SND_Init();

    SeqInit(SEQ_CLOCK_INTERVAL_NITRO_VBLANK, MidiCallback);
    ChInit();

    // V�u�����N���荞�ݐݒ�
    OS_SetIrqFunction(OS_IE_V_BLANK, VBlankIntr);
    (void)OS_EnableIrqMask(OS_IE_V_BLANK);
    (void)OS_EnableIrq();
    (void)GX_VBlankIntr(TRUE);

    // print usage
    OS_Printf("=================================\n");
    OS_Printf("USAGE:\n");
    OS_Printf(" A : start sequence\n");
    OS_Printf(" B : stop sequence\n");
    OS_Printf("=================================\n");

    // MIDI�t�@�C�������[�h
    FileLoad(midifile_buf, "/elise.mid");

    while (1)
    {
        u16     ReadData;

        OS_WaitVBlankIntr();

        // �`�q�l�V�R�}���h������M
        while (SND_RecvCommandReply(SND_COMMAND_NOBLOCK) != NULL)
        {
        }

        // �L�[���͓ǂݍ���
        ReadData = PAD_Read();
        Trg = (u16)(ReadData & (ReadData ^ Cont));
        Cont = ReadData;

        if (Trg & PAD_BUTTON_A)
        {
            ChAllNoteOff();
            (void)SeqPlay(&handle, midifile_buf);
        }

        if (Trg & PAD_BUTTON_B)
        {
            (void)SeqStop(&handle);
            ChAllNoteOff();
        }

        (void)SeqMain(&handle);

        // �R�}���h�t���b�V���i�t���b�V�����đ����Ɏ��s��v���j
        (void)SND_FlushCommand(SND_COMMAND_NOBLOCK | SND_COMMAND_IMMEDIATE);
    }
}

void VBlankIntr(void)
{
    OS_SetIrqCheckFlag(OS_IE_V_BLANK); // checking VBlank interrupt
}

void FileLoad(void *buf, const char *filename)
{
    FSFile  file;
    u32     file_size;

    FS_InitFile(&file);
    (void)FS_OpenFile(&file, filename);
    file_size = FS_GetLength(&file);
    (void)FS_ReadFile(&file, buf, (s32)file_size);
    (void)FS_CloseFile(&file);
}

/*---------------------------------------------------------------------------*
  Name:         MidiCallback

  Description:  SeqMain() �ŃV�[�P���X��������
                MIDI�C�x���g�������������ɌĂяo�����֐�

  Arguments:    midi_event - ��������MIDI�C�x���g��

  Returns:      None
 *---------------------------------------------------------------------------*/
void MidiCallback(const u8 *midi_event)
{
    static u8 running_status;
    u8      midi_buf[4];
    int     i;

    // MIDI�C�x���g�ʂ̃o�C�g�T�C�Y
    const u8 midi_byte_size[8] = {
        /*  8x,9x,Ax,Bx,Cx,Dx,Ex,Fx */
        2, 2, 2, 2, 1, 1, 2, 0
    };

    // �����j���O�X�e�[�^�X�̃`�F�b�N
    if (midi_event[0] < 0x80)
    {
        midi_buf[0] = running_status;
        for (i = 0; i < midi_byte_size[(midi_buf[0] >> 4) - 8]; i++)
        {
            midi_buf[i + 1] = midi_event[i];
        }
    }
    else
    {
        running_status = midi_event[0];
        midi_buf[0] = midi_event[0];
        for (i = 0; i < midi_byte_size[(midi_buf[0] >> 4) - 8]; i++)
        {
            midi_buf[i + 1] = midi_event[i + 1];
        }
    }

    ChSetEvent(midi_buf);
}
