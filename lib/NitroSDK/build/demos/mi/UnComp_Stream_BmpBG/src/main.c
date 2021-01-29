/*---------------------------------------------------------------------------*
  Project:  NitroSDK - demos - MI - UnComp_Stream_BmpBG
  File:     main.c

  Copyright 2003-2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: main.c,v $
  Revision 1.16  2006/01/18 02:11:29  kitase_hirotake
  do-indent

  Revision 1.15  2005/11/21 10:53:27  kitase_hirotake
  SVC_WaitVBlankIntr �� OS_WaitVBlankIntr �ɕύX

  Revision 1.14  2005/06/27 11:10:12  yosizaki
  add comment about DC_WaitWriteBufferEmpty().

  Revision 1.13  2005/02/28 05:26:19  yosizaki
  do-indent.

  Revision 1.12  2005/02/18 13:47:49  seiki_masashi
  warning �΍�

  Revision 1.11  2005/01/18 00:20:07  takano_makoto
  fix Copyright header.

  Revision 1.10  2005/01/14 06:03:23  takano_makoto
  ���ʂȃL���b�V���N���A���폜

  Revision 1.9  2004/12/08 05:28:01  takano_makoto
  �W�J�p�o�b�t�@��32�o�C�g�A���C���ɑ�����B

  Revision 1.8  2004/12/08 04:29:00  takano_makoto
  �t�@�C���̔񓯊����[�h�̂��߂ɓǂݍ��݈ʒu��512�o�C�g�A���C���ɑ�����

  Revision 1.7  2004/12/07 12:29:31  takano_makoto
  �L���b�V�����t���b�V������悤�ɏC��

  Revision 1.6  2004/12/01 08:11:12  takano_makoto
  CARD��񓯊����[�h����T���v����ǉ�

  Revision 1.5  2004/12/01 00:02:27  takano_makoto
  �֐����� MI_UncompReadXX -> MI_ReadUncompXX�֕ύX

  Revision 1.4  2004/11/30 14:07:39  takano_makoto
  ����VRAM�֓W�J����T���v����ǉ��B

  Revision 1.3  2004/11/30 08:36:04  takano_makoto
  MI_InitUncompContextHuff -> MI_InitUncompContextHuffman�ɖ��̕ύX

  Revision 1.2  2004/11/30 08:01:37  takano_makoto
  small fix in comment.

  Revision 1.1  2004/11/30 07:59:58  takano_makoto
  Initial update.

  $NoKeywords: $
 *---------------------------------------------------------------------------*/

//---------------------------------------------------------------------------
// A sample that displays image uncompressed by streaming:
//
// If display mode is VRAM display mode, a bitmap image on VRAM is displayed.
// This sample loads different four images compressed different format to VRAM A-D,
// and displays them by turns.
//
//---------------------------------------------------------------------------

#include <nitro.h>
#include <nitro/mi/uncomp_stream.h>
#include "DEMO.h"

#define TEMP_BUF_SIZE   512

static u8 *data_uncomp_buf;
static u8 doubleBuf[2][TEMP_BUF_SIZE] ATTRIBUTE_ALIGN(32);

static void InitAlloc(void)
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


void NitroMain(void)
{
    int     vram_slot = 0, count = 0;
    u8     *tmpBuf = doubleBuf[0];

    //---------------------------------------------------------------------------
    // Initialize:
    // They enable IRQ interrupts, and initialize VRAM.
    //---------------------------------------------------------------------------
    DEMOInitCommon();
    DEMOInitVRAM();
    InitAlloc();
    FS_Init(3);
    OS_InitTick();

    //---------------------------------------------------------------------------
    // Map VRAM bank A-D onto LCDC.
    //---------------------------------------------------------------------------
    GX_SetBankForLCDC(GX_VRAM_LCDC_A | GX_VRAM_LCDC_B | GX_VRAM_LCDC_C | GX_VRAM_LCDC_D);

    //---------------------------------------------------------------------------
    // Download images
    //---------------------------------------------------------------------------

    //===========================
    // �����k�f�[�^
    //===========================
    {
        FSFile  file;
        u32     read_len;

        // �t�@�C���̃I�[�v��
        FS_InitFile(&file);
        if (!FS_OpenFile(&file, "/data/image1.bin"))
        {
            OS_TPrintf(" Open File /data/image1.bin fail\n");
            return;
        }
        // �f�[�^�ǂݍ��ݗ̈�̊m��
        read_len = FS_GetLength(&file);
        data_uncomp_buf = (u8 *)OS_Alloc(read_len);

        // �t�@�C���]��
        (void)FS_ReadFile(&file, data_uncomp_buf, 256 * 192 * 2);

        // VRAM�ɓ]��
        MI_DmaCopy32(3, data_uncomp_buf, (void *)HW_LCDC_VRAM_A,
                     256 * 192 * sizeof(unsigned short));

        OS_Free(data_uncomp_buf);
        (void)FS_CloseFile(&file);

    }

    //===========================
    // ���������O�X���k�f�[�^
    //===========================
    {
        FSFile  file;
        MIUncompContextRL context;
        s32     read_len;
        u32     data_size;

        // �t�@�C���̃I�[�v��
        FS_InitFile(&file);
        if (!FS_OpenFile(&file, "/data/image2_RL.bin"))
        {
            OS_TPrintf(" Open File /data/image2_RL.bin fail\n");
            return;
        }

        // ���k�f�[�^�̃w�b�_�������擾
        (void)FS_ReadFile(&file, tmpBuf, sizeof(MICompressionHeader));

        // �f�[�^�W�J�̈�̊m��
        data_size = MI_GetUncompressedSize(tmpBuf);
        data_uncomp_buf = (u8 *)OS_Alloc(data_size);

        // �R���e�L�X�g�̏�����
        MI_InitUncompContextRL(&context, data_uncomp_buf, (MICompressionHeader *)tmpBuf);

        while (1)
        {
            // �C�ӂ̃T�C�Y��ǂݍ��݃X�g���[�~���O�W�J���s�Ȃ��܂��B
            read_len = FS_ReadFile(&file, tmpBuf, TEMP_BUF_SIZE);
            if (read_len <= 0)
            {
                OS_TPanic("can't read file\n");
            }
            // ��������ɓW�J����ꍇ�͍�����8bit�A�N�Z�X�֐����g�p���܂��B
            if (MI_ReadUncompRL8(&context, tmpBuf, (u32)read_len) == 0)
            {
                break;
            }
        }

        // VRAM�ɓ]��
        DC_FlushRange(data_uncomp_buf, data_size);      // �L���b�V�����t���b�V��
        /* DMA�����IO���W�X�^�փA�N�Z�X����̂ŃL���b�V���� Wait �͕s�v */
        // DC_WaitWriteBufferEmpty();
        MI_DmaCopy32(3, data_uncomp_buf, (void *)HW_LCDC_VRAM_B,
                     256 * 192 * sizeof(unsigned short));

        OS_Free(data_uncomp_buf);
        (void)FS_CloseFile(&file);

    }

    //===========================
    // LZ77���k�f�[�^ - VRAM�֒��ړW�J����T���v��
    //===========================
    {
        FSFile  file;
        MIUncompContextLZ context;
        s32     read_len;

        // �t�@�C���̃I�[�v��
        FS_InitFile(&file);
        if (!FS_OpenFile(&file, "/data/image3_LZ.bin"))
        {
            OS_TPrintf(" Open File /data/image3_LZ.bin fail\n");
            return;
        }

        // ���k�f�[�^�̃w�b�_�������擾
        (void)FS_ReadFile(&file, tmpBuf, sizeof(MICompressionHeader));

        // VRAM�֒��ړW�J����ꍇ�̃R���e�L�X�g�̏������Adest�A�h���X��VRAM���w��
        MI_InitUncompContextLZ(&context, (void *)HW_LCDC_VRAM_C, (MICompressionHeader *)tmpBuf);

        while (1)
        {
            // �C�ӂ̃T�C�Y��ǂݍ��݃X�g���[�~���O�W�J���s�Ȃ�
            read_len = FS_ReadFile(&file, tmpBuf, TEMP_BUF_SIZE);
            if (read_len <= 0)
            {
                OS_TPanic("can't read file\n");
            }

            // VRAM�֒��ړW�J����ꍇ��16bit�A�N�Z�X�֐����g�p����B
            if (MI_ReadUncompLZ16(&context, tmpBuf, (u32)read_len) == 0)
            {
                break;
            }
        }

        (void)FS_CloseFile(&file);
    }

    //===========================
    // �n�t�}�����k�f�[�^ - �J�[�h����f�[�^��񓯊����[�h���Ȃ�����s���ēW�J���s�Ȃ��T���v��
    //===========================
    {
        FSFile  file;
        MIUncompContextHuffman context;
        u32     data_size;
        s32     read_len[2];
        u8      read_select;
        u8      uncomp_select;

        // �t�@�C���̃I�[�v��
        FS_InitFile(&file);
        if (!FS_OpenFile(&file, "/data/image4_HUFF.bin"))
        {
            OS_TPrintf(" Open File /data/image4_HUFF.bin fail\n");
            return;
        }

        // ���k�f�[�^�̃w�b�_�������擾
        (void)FS_ReadFile(&file, tmpBuf, sizeof(MICompressionHeader));

        // �f�[�^�W�J�̈�̊m��
        data_size = MI_GetUncompressedSize(tmpBuf);
        data_uncomp_buf = (u8 *)OS_Alloc(data_size);

        // �R���e�L�X�g�̏�����
        MI_InitUncompContextHuffman(&context, data_uncomp_buf, (MICompressionHeader *)tmpBuf);

        read_select = 0;
        uncomp_select = 1;
        read_len[0] = 0;
        read_len[1] = 0;

        // �t�@�C���̔񓯊����[�h���s�Ȃ����߂ɁA�ǂݍ��݈ʒu��512Byte�A���C���ɍ��킹��K�v������܂��B
        // �w�b�_������4Byte���s���ēǂݍ���ł���̂ŁA�c�� 512 - 4Byte�ǂݍ���ł����܂��B
        read_len[read_select] =
            FS_ReadFileAsync(&file, doubleBuf[read_select], 512 - sizeof(MICompressionHeader));

        /* �J�[�h��񓯊��Ƀ��[�h���Ȃ�����񂵂ēW�J���s�Ȃ��܂� */
        while (1)
        {
            // �_�u���o�b�t�@�̐؂�ւ�
            read_select ^= 0x1;
            uncomp_select ^= 0x1;

            // �J�[�h�̓ǂݍ��݊�����҂��܂��B
            (void)FS_WaitAsync(&file);

            // �C�ӂ̃T�C�Y��ǂݍ��݃X�g���[�~���O�W�J���s�Ȃ��܂��B
            read_len[read_select] = FS_ReadFileAsync(&file, doubleBuf[read_select], TEMP_BUF_SIZE);

            if (read_len[uncomp_select] == 0)
            {
                continue;
            }
            if (read_len[uncomp_select] == -1)
            {
                OS_TPanic("can't read file\n");
            }

            if (MI_ReadUncompHuffman
                (&context, doubleBuf[uncomp_select], (u32)read_len[uncomp_select]) == 0)
            {
                // �f�[�^�W�J����
                break;
            }
        }

        // VRAM�ɓ]��
        DC_FlushRange(data_uncomp_buf, data_size);      // �L���b�V�����t���b�V��
        /* DMA�����IO���W�X�^�փA�N�Z�X����̂ŃL���b�V���� Wait �͕s�v */
        // DC_WaitWriteBufferEmpty();
        MI_DmaCopy32(3, data_uncomp_buf, (void *)HW_LCDC_VRAM_D,
                     256 * 192 * sizeof(unsigned short));

        OS_Free(data_uncomp_buf);
        (void)FS_CloseFile(&file);
    }

    //---------------------------------------------------------------------------
    // Set graphics mode VRAM display mode
    //---------------------------------------------------------------------------
    GX_SetGraphicsMode(GX_DISPMODE_VRAM_A,      // display VRAM-A
                       (GXBGMode)0,    // dummy
                       (GXBG0As)0);    // dummy

    DEMOStartDisplay();
    while (1)
    {

#ifdef SDK_AUTOTEST
        EXT_TestScreenShot(100, 0xEFA8B182);
        EXT_TestTickCounter();
#endif //SDK_AUTOTEST

        OS_WaitVBlankIntr();           // Waiting the end of VBlank interrupt

        //---------------------------------------------------------------------------
        // Change the VRAM slot displayed every 90 frames
        //---------------------------------------------------------------------------
        if (count++ > 90)
        {
            vram_slot++;
            vram_slot &= 0x03;
            switch (vram_slot)
            {
            case 0:
                GX_SetGraphicsMode(GX_DISPMODE_VRAM_A,  // display VRAM-A
                                   (GXBGMode)0, // dummy
                                   (GXBG0As)0); // dummy
                break;
            case 1:
                GX_SetGraphicsMode(GX_DISPMODE_VRAM_B,  // display VRAM-B
                                   (GXBGMode)0, // dummy
                                   (GXBG0As)0); // dummy
                break;
            case 2:
                GX_SetGraphicsMode(GX_DISPMODE_VRAM_C,  // display VRAM-C
                                   (GXBGMode)0, // dummy
                                   (GXBG0As)0); // dummy
                break;
            case 3:
                GX_SetGraphicsMode(GX_DISPMODE_VRAM_D,  // display VRAM-D
                                   (GXBGMode)0, // dummy
                                   (GXBG0As)0); // dummy
                break;
            }
            // reset a counter
            count = 0;
        }
    }
}

//---------------------------------------------------------------------------
// VBlank interrupt function:
//
// Interrupt handlers are registered on the interrupt table by OS_SetIRQFunction.
// OS_EnableIrqMask selects IRQ interrupts to enable, and
// OS_EnableIrq enables IRQ interrupts.
// Notice that you have to call 'OS_SetIrqCheckFlag' to check a VBlank interrupt.
//---------------------------------------------------------------------------
void VBlankIntr(void)
{
    OS_SetIrqCheckFlag(OS_IE_V_BLANK); // checking VBlank interrupt
}
