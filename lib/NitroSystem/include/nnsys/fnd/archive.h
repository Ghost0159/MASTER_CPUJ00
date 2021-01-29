/*---------------------------------------------------------------------------*
  Project:  NITRO-System - include - nnsys - fnd
  File:     archive.h

  Copyright 2004-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Revision: 1.4 $
 *---------------------------------------------------------------------------*/

#ifndef NNS_FND_ARCHIVE_H_
#define NNS_FND_ARCHIVE_H_

//#include <stddef.h>
#include <nitro/os.h>
#include <nitro/fs.h>


#ifdef __cplusplus
extern "C" {
#endif


/*---------------------------------------------------------------------------*
  Name:         NNSFndArchiveFatEntry

  Description:  �A�[�J�C�u�̂e�`�s�G���g���\���́B
 *---------------------------------------------------------------------------*/
typedef struct
{
	u32						fileTop;			// �t�@�C���C���[�W�擪�̃I�t�Z�b�g
	u32						fileBottom;			// �t�@�C���C���[�W����̃I�t�Z�b�g

} NNSiFndArchiveFatEntry;


/*---------------------------------------------------------------------------*
  Name:         NNSiFndArchiveFatData

  Description:  �A�[�J�C�u��FAT�f�[�^�\���́B
 *---------------------------------------------------------------------------*/
typedef struct
{
	u32						blockType;			// �u���b�N�^�C�v
	u32						blockSize;			// �u���b�N�T�C�Y
	u16						numFiles;			// �t�@�C���̌��B
	u16						reserved;			// �\��B
	NNSiFndArchiveFatEntry	fatEntries[1];

} NNSiFndArchiveFatData;


/*---------------------------------------------------------------------------*
  Name:         NNSiFndArchiveHeader

  Description:  �A�[�J�C�u�̃t�@�C���w�b�_�\���́B
 *---------------------------------------------------------------------------*/
typedef struct
{
	u32						signature;			// �V�O�l�`���iNARC�j
	u16						byteOrder;			// �o�C�g�I�[�_�[�i0xfeff�j
	u16						version;			// �o�[�W�����ԍ�
	u32						fileSize;			// �A�[�J�C�u�t�@�C���̑傫��
	u16						headerSize;			// �A�[�J�C�u�t�@�C���w�b�_�̑傫��
	u16						dataBlocks;			// �f�[�^�u���b�N�̐�

} NNSiFndArchiveHeader;


/*---------------------------------------------------------------------------*
  Name:         NNSFndArchive

  Description:  NNS�A�[�J�C�u�\���́B�A�[�J�C�u�Ɋւ������ێ����܂��B
  				NITRO-System�̃A�[�J�C�u�}�l�[�W���ŁA�A�[�J�C�u����肷���
  				���́A����NNS�A�[�J�C�u�\���̂��w�肵�܂��B
 *---------------------------------------------------------------------------*/
typedef struct
{
	FSArchive				fsArchive;			// �t�@�C���V�X�e���p���[�N
	NNSiFndArchiveHeader*	arcBinary;			// �A�[�J�C�u�o�C�i���f�[�^�̐擪
	NNSiFndArchiveFatData*	fatData;			// �A�[�J�C�u��FAT�f�[�^�̐擪
	u32						fileImage;			// �A�[�J�C�u�̃t�@�C���C���[�W�̐擪

} NNSFndArchive;


/*---------------------------------------------------------------------------*
    �֐��v���g�^�C�v�B

 *---------------------------------------------------------------------------*/

BOOL		NNS_FndMountArchive(
					NNSFndArchive*		archive,
					const char*			arcName,
					void*				arcBinary);

BOOL		NNS_FndUnmountArchive(
					NNSFndArchive*		archive);

void*		NNS_FndGetArchiveFileByName(
					const char*			path);

void*		NNS_FndGetArchiveFileByIndex(
					NNSFndArchive*		archive,
					u32					index);

BOOL		NNS_FndOpenArchiveFileByIndex(
					FSFile*				file,
					NNSFndArchive*		archive,
					u32					index);


#ifdef __cplusplus
} /* extern "C" */
#endif

/* NNS_FND_ARCHIVE_H_ */
#endif
