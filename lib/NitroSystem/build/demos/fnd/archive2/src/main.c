/*---------------------------------------------------------------------------*
  Project:  NITRO-System - demos - fnd - archive2
  File:     main.c

  Copyright 2004-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Revision: 1.6 $
 *---------------------------------------------------------------------------*/

#include "sdk_init.h"
#include "nns_util.h"

// �A�[�J�C�u�̃C���f�b�N�X��`�t�@�C��
#include "picture.naix"


static void	LoadPicture(void);

/* -------------------------------------------------------------------------
  Name:         NitroMain

  Description:  ���k���ꂽ�A�[�J�C�u����a�f�f�[�^��ǂݍ��݁A��ʂ�\����
				��T���v���v���O�����ł��B

  Arguments:    �Ȃ��B

  Returns:      �Ȃ��B
   ------------------------------------------------------------------------- */
void
NitroMain(void)
{
    InitSystem();
	InitMemory();
    InitVRAM();

    InitDisplay();
	LoadPicture();
	G2_SetBG0Offset(0,20);

    for(;;)
    {
		SVC_WaitVBlankIntr();
		ReadGamePad();
	}
}

/*---------------------------------------------------------------------------*
  Name:         LoadPicture

  Description:  ���k���ꂽ�A�[�J�C�u���P���[�h���A���̒��Ɋi�[����Ă���
  				�L�����N�^�A�J���[�p���b�g�A�X�N���[���̊e�f�[�^��VRAM�ɓ]��
  				���܂��B

  				�e�f�[�^��VRAM�ɓ]��������A�A�[�J�C�u���폜���܂��B

  Arguments:    �Ȃ��B

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
static void
LoadPicture(void)
{
	NNSFndArchive* archive;

	// ���k���ꂽ�A�[�J�C�u�����C���������ɓW�J���A�t�@�C���V�X�e���Ƀ}�E���g���܂��B
	// LoadCompressedArchive()����DC_FlushRange()�̌Ăяo�����s���Ă��܂��B
	// LoadCompressedArchive()�́Anns_util.c���ɂ���܂��B
	if ((archive = LoadCompressedArchive("PCT", "/data/picture_LZ.bin")) != NULL)
	{
		// �t�@�C�������w�肵�ăf�[�^���擾�B
		void* chrData = NNS_FndGetArchiveFileByName("PCT:/picture/flower/flower256.chr");
		void* scnData = NNS_FndGetArchiveFileByName("PCT:/picture/flower/flower256.scn");

		// �C���f�b�N�X���w�肵�ăf�[�^���擾�B
		void* palData = NNS_FndGetArchiveFileByIndex(archive, NARC_picture_flower256_pal);

		// �f�[�^��VRAM�̃��[�h�B
		GX_LoadBG0Char(chrData, 0, 0x10000);
    	GX_LoadBG0Scr (scnData, 0, 0x00800);
		GX_LoadBGPltt (palData, 0, 0x00200);

		// VRAM�Ƀf�[�^�����[�h������́A���C����������̃f�[�^�͕s�v�ȈׁA
		// �A�[�J�C�u���폜���܂��B
		// RemoveArchive()�́Anns_util.c���ɂ���܂��B
		RemoveArchive(archive);
	}
}
