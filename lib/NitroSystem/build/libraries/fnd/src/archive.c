/*---------------------------------------------------------------------------*
  Project:  NITRO-System - libraries - fnd
  File:     archive.c

  Copyright 2004-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Revision: 1.6 $
 *---------------------------------------------------------------------------*/

#include <nnsys/misc.h>
#include <nnsys/fnd/archive.h>
#include <string.h>

#include "archive_block.h"


/*---------------------------------------------------------------------------*
  Name:         IsValidArchiveImage

  Description:  �w�肳�ꂽ�A�[�J�C�u�w�b�_�̓��e�𒲂ׁA�A�[�J�C�u�o�C�i����
  				�L���Ȃ��̂ł��邩�𔻒肵�܂��B

  Arguments:    arcBinary:	�A�[�J�C�u�w�b�_�ւ̃|�C���^�B

  Returns:      �����L���ȃA�[�J�C�u�w�b�_�ł���΁A�^��Ԃ��܂��B
 *---------------------------------------------------------------------------*/
static BOOL
IsValidArchiveBinary(NNSiFndArchiveHeader* arcBinary)
{
	NNS_NULL_ASSERT(arcBinary);

	if (arcBinary->signature != 'CRAN')  return FALSE;
	if (arcBinary->byteOrder != 0xfffe)  return FALSE;
	if (arcBinary->version   != 0x0100)  return FALSE;

	return TRUE;
}

/*---------------------------------------------------------------------------*
  Name:         NNS_FndMountArchive

  Description:  �w�肳�ꂽ�A�[�J�C�u�o�C�i�����t�@�C���V�X�e���Ƀ}�E���g����
  				���B�A�[�J�C�u���}�E���g����ꍇ�ɂ́A�R�����܂ł̎��ʖ����w
  				�肵�܂��B�t�@�C���V�X�e����ŃA�[�J�C�u����肷��ꍇ�ɂ́A
  				�p�X���̑O�ɂ��̎��ʖ����L�q���邱�ƂɂȂ�܂��B

  Arguments:    archive:	NNS�A�[�J�C�u�\���̂ւ̃|�C���^�B
  				arcName:	�A�[�J�C�u���t�@�C���V�X�e����Ŏ��ʂ���ׂ̖��O�B
  				arcBinary:	��������ɓǂݏo����Ă���A�[�J�C�u�o�C�i���ւ�
  							�|�C���^�B
  				
  Returns:      �����}�E���g�ɐ�������΁A�^��Ԃ��܂��B
 *---------------------------------------------------------------------------*/
BOOL
NNS_FndMountArchive(NNSFndArchive* archive, const char* arcName, void* arcBinary)
{
	NNSiFndArchiveHeader*		  arc = (NNSiFndArchiveHeader*)arcBinary;
	NNSiFndArchiveFatBlockHeader* fat = NULL;
	NNSiFndArchiveDirBlockHeader* fnt = NULL;
	NNSiFndArchiveImgBlockHeader* img = NULL;

	NNS_NULL_ASSERT(arcName);
	NNS_NULL_ASSERT(arcBinary);

	if (!IsValidArchiveBinary(arc))
	{
		OS_Printf("This archive binary is invalid.\n");
		return FALSE;
	}
	{
		int  count;
		NNSiFndArchiveBlockHeader* block = (NNSiFndArchiveBlockHeader*)((u32)arc + arc->headerSize);

		for (count=0; count<arc->dataBlocks; count++)
		{
			switch (block->blockType)
			{
				case 'FATB': fat = (NNSiFndArchiveFatBlockHeader*)block;	break;
				case 'FNTB': fnt = (NNSiFndArchiveDirBlockHeader*)block;	break;
				case 'FIMG': img = (NNSiFndArchiveImgBlockHeader*)block;	break;
			}
			block = (NNSiFndArchiveBlockHeader*)((u32)block + block->blockSize);
		}

		FS_InitArchive(&archive->fsArchive);
		archive->arcBinary = arc;
		archive->fatData   = (NNSiFndArchiveFatData*)fat;
		archive->fileImage = (u32)(img+1);

		if (!FS_RegisterArchiveName(&archive->fsArchive, arcName, strlen(arcName)))
		{
			OS_Printf("archive cannot register to file system.\n");
			return FALSE;
		}
		if (!FS_LoadArchive(&archive->fsArchive,
					(u32)(img+1),
					(u32)(fat+1) - (u32)(img+1), fat->blockSize-sizeof(NNSiFndArchiveFatBlockHeader),
					(u32)(fnt+1) - (u32)(img+1), fnt->blockSize-sizeof(NNSiFndArchiveDirBlockHeader),
					NULL, NULL)
		)
		{
			OS_Printf("archive cannot load to file system.\n");
			FS_ReleaseArchiveName(&archive->fsArchive);
			return FALSE;
		}
	}
	return TRUE;
}

/*---------------------------------------------------------------------------*
  Name:         NNS_FndUnmountArchive

  Description:  �t�@�C���V�X�e���Ƀ}�E���g����Ă���A�[�J�C�u���A���}�E���g
  				���܂��B

  Arguments:    archive:	NNS�A�[�J�C�u�\���̂ւ̃|�C���^�B

  Returns:      �����A�A���}�E���g�ɐ�������΁A�^��Ԃ��܂��B
 *---------------------------------------------------------------------------*/
BOOL
NNS_FndUnmountArchive(NNSFndArchive* archive)
{
	NNS_NULL_ASSERT(archive);

	if (!FS_UnloadArchive(&archive->fsArchive))
	{
		return FALSE;
	}
	FS_ReleaseArchiveName(&archive->fsArchive);
	return TRUE;
}

/*---------------------------------------------------------------------------*
  Name:         NNS_FndGetArchiveFileByName

  Description:  �t�@�C���V�X�e�����g�p���āA�w�肳�ꂽ�p�X�ɑΉ�����A�[�J�C
  				�u�t�@�C�����i�[����Ă���q�`�l�A�h���X���擾���܂��B

  Arguments:    path:	�A�[�J�C�u�t�@�C���̃p�X���B

  Returns:      �A�[�J�C�u�t�@�C�����i�[����Ă���A�h���X�B
 *---------------------------------------------------------------------------*/
void*
NNS_FndGetArchiveFileByName(const char* path)
{
	void* addr = NULL;

#if 1
	FSFile file;

	NNS_NULL_ASSERT(path);
	FS_InitFile(&file);
	if (FS_OpenFile(&file, path))
	{
		NNSFndArchive* archive = (NNSFndArchive*)FS_GetAttachedArchive(&file);

		addr = (void*)(archive->fileImage + FS_GetFileImageTop(&file));
		(void)FS_CloseFile(&file);
	}
#else
	FSFileID fileID;

	NNS_NULL_ASSERT(path);
	if (FS_ConvertPathToFileID(&fileID, path))
	{
		NNSFndArchive* archive = (NNSFndArchive*)fileID.arc;

		addr = (void*)(archive->fileImage + archive->fatData->fatEntries[fileID.file_id].fileTop);
	}
#endif

	return addr;
}

/*---------------------------------------------------------------------------*
  Name:         NNS_FndGetArchiveFileByIndex

  Description:  �C���f�b�N�X�ɂ��w�肳�ꂽ�A�[�J�C�u�t�@�C�����i�[����Ă�
  				��q�`�l�A�h���X���擾���܂��B

  Arguments:    archive:	NNS�A�[�J�C�u�\���̂ւ̃|�C���^�B
  				index:		�A�[�J�C�u���̃t�@�C���̃C���f�b�N�X�l�B

  Returns:      �A�[�J�C�u�t�@�C�����i�[����Ă���A�h���X��Ԃ��܂��B�����A
  				�w�肵���C���f�b�N�X�ɑΉ����閳�������ꍇ�ɂ́ANULL��Ԃ���
  				���B
 *---------------------------------------------------------------------------*/
void*
NNS_FndGetArchiveFileByIndex(NNSFndArchive* archive, u32 index)
{
	void* addr = NULL;

	NNS_NULL_ASSERT(archive);

	if (index < archive->fatData->numFiles)
	{
		addr = (void*)(archive->fileImage + archive->fatData->fatEntries[index].fileTop);
	}
	return addr;
}

/*---------------------------------------------------------------------------*
  Name:         NNS_FndOpenArchiveFileByIndex

  Description:  �C���f�b�N�X�ɂ��w�肳�ꂽ�A�[�J�C�u�t�@�C�����I�[�v������
  				���B

  Arguments:	file:		�t�@�C���\���̂ւ̃|�C���^�B
  			    archive:	NNS�A�[�J�C�u�\���̂ւ̃|�C���^�B
  				index:		�A�[�J�C�u���̃t�@�C���̃C���f�b�N�X�l�B

  Returns:      �t�@�C���̃I�[�v���ɐ�������ΐ^��Ԃ��܂��B
 *---------------------------------------------------------------------------*/
BOOL
NNS_FndOpenArchiveFileByIndex(FSFile* file, NNSFndArchive* archive, u32 index)
{
	BOOL result = FALSE;

	NNS_NULL_ASSERT(file);
	NNS_NULL_ASSERT(archive);

	if (index < archive->fatData->numFiles)
	{
		FSFileID fileID;

		fileID.arc	   = &archive->fsArchive;
		fileID.file_id = index;

		result = FS_OpenFileFast(file, fileID);
	}
	return result;
}
