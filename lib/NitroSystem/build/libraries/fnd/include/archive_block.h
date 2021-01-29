/*---------------------------------------------------------------------------*
  Project:  NITRO-System - libraries - fnd
  File:     archive_block.h

  Copyright 2004-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Revision: 1.3 $
 *---------------------------------------------------------------------------*/

#ifndef NNS_FND_ARCHIVE_BLOCK_H_
#define NNS_FND_ARCHIVE_BLOCK_H_

#ifdef __cplusplus
extern "C" {
#endif

/*---------------------------------------------------------------------------*
  Name:         NNSiFndArchiveBlockHeader

  Description:  �A�[�J�C�u�t�@�C���̃f�[�^�u���b�N�w�b�_�\���́B
 *---------------------------------------------------------------------------*/
typedef struct
{
	u32			blockType;				// �u���b�N�^�C�v
	u32			blockSize;				// �u���b�N�T�C�Y

} NNSiFndArchiveBlockHeader;


/*---------------------------------------------------------------------------*
  Name:         NNSiFndArchiveDirBlock
  				NNSiFndArchiveImgBlock

  Description:  �A�[�J�C�u�t�@�C���̃f�B���N�g�����u���b�N�w�b�_�\���́B
  				�A�[�J�C�u�t�@�C���̃t�@�C���C���[�W�u���b�N�w�b�_�\���́B
 *---------------------------------------------------------------------------*/

typedef	NNSiFndArchiveBlockHeader	NNSiFndArchiveDirBlockHeader;
typedef	NNSiFndArchiveBlockHeader	NNSiFndArchiveImgBlockHeader;


/*---------------------------------------------------------------------------*
  Name:         NNSFndArchiveFatBlock

  Description:  �A�[�J�C�u�t�@�C����FAT�u���b�N�w�b�_�\���́B
 *---------------------------------------------------------------------------*/
typedef struct
{
	u32			blockType;				// �u���b�N�^�C�v
	u32			blockSize;				// �u���b�N�T�C�Y
	u16			numFiles;				// �t�@�C���̌��B
	u16			reserved;				// �\��B

} NNSiFndArchiveFatBlockHeader;


#ifdef __cplusplus
} /* extern "C" */
#endif

/* NNS_FND_ARCHIVE_BLOCK_H_ */
#endif
