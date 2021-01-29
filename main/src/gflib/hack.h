/*---------------------------------------------------------------------------*
  Project:  NitroSDK - TRIAL - tests - rebooter
  File:     hack.h

  Copyright 2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: main.c,v $
  $NoKeywords: $
 *---------------------------------------------------------------------------*/
#ifndef NITROSDK_SILESYSTEM_HACK_HEADER__
#define NITROSDK_SILESYSTEM_HACK_HEADER__

#include <nitro.h>

// ��[����] �|�P�����Ńo�g������v���O���������炤���́A
//          ���ׂĂ���"ADAJ"�ɂ��Ă����B����Ńo�g������̃v���O�����ł�
//          �K������"ADAJ"�Ń`�F�b�N����B
//          �Ȃ̂ŁA�ύX���Ă͂����Ȃ��I�I
#define GAME_CODE_ADAJ     (('A' << 0) | ('D' << 8) | ('A' << 16) | ('J' << 24))
#define MAKER_CODE_01      (('0' << 0) | ('1' << 8))

/*****************************************************************************/
/* function */

/*
 * FS���C�u�����W����ROM�A�[�J�C�u���w���t�@�C���V�X�e�����A
 * �w���݂̃v���O�����x�łȂ��w�{�̃v���O�����x�̂��̂ɕ␳���܂��B
 * ����Ƀ|�P�����_�C�A�����h�̃C�j�V�����R�[�h���g�p���āAROM�̐�������
 * �`�F�b�N���Ă��܂��B(�ł��̂ŁA�{�֐��̓|�P������p�֐��ł��B)
 * �{�̃v���O�������g���炱�̊֐����Ăяo�����ꍇ�́A
 * ���̏������s���܂��B
 *
 * ���̊֐��� FS_Init �֐��Ăяo���̒���ɌĂяo���K�v������܂��B
 */
SDK_INLINE
void NormalizeRomArchive(void)
{
    if (!FS_IsAvailable())
    {
        OS_TPanic("no archive to replace!");
    }
    else
    {
        static const char name[] = "rom";
        static const int name_len = sizeof(name) - 1;
        const CARDRomHeader* header;
        FSArchive *rom;
        {
            CARDRomHeader * const arg_buffer = (CARDRomHeader *)0x027FF000/*HW_MAIN_MEM_SHARED*/;
            CARDRomHeader * const app_header = (CARDRomHeader *)HW_ROM_HEADER_BUF;
            CARDRomHeader * const org_header = (CARDRomHeader *)HW_CARD_ROM_HEADER;
            if (arg_buffer->game_code == 0)
            {
                // ROM�w�b�_�̓��e��ޔ�̈�ɃR�s�[���܂��B
                CARD_Init();
                MI_CpuCopy8(app_header, arg_buffer, HW_CARD_ROM_HEADER_SIZE);
                MI_CpuCopy8(app_header, org_header, HW_CARD_ROM_HEADER_SIZE);
                arg_buffer->game_code = GAME_CODE_ADAJ;
            }
            header = arg_buffer;
        }
        rom = FS_FindArchive(name, name_len);
        rom->fat = header->fat.offset;
        rom->fat_size = header->fat.length;
        rom->fnt = header->fnt.offset;
        rom->fnt_size = header->fnt.length;
        if (header->game_code != GAME_CODE_ADAJ ||
            header->maker_code != MAKER_CODE_01)
        {
            // �ʏ킠�蓾�Ȃ����A�o�g������󂯎��ׂ�ROM�łȂ��B�m�f�B
            OS_TPanic("illegal rom");
        }
    }
}

#endif /* NITROSDK_SILESYSTEM_HACK_HEADER__ */
