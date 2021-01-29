/*---------------------------------------------------------------------------*
  Project:  NitroSDK - MB - libraries
  File:     mb_fileinfo.h

  Copyright 2003-2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: mb_fileinfo.h,v $
  Revision 1.13  2006/01/18 02:11:30  kitase_hirotake
  do-indent

  Revision 1.12  2005/02/28 05:26:25  yosizaki
  do-indent.

  Revision 1.11  2004/09/25 12:10:38  sato_masaki
  MBi_IsAbleToRecv()��ǉ��B(IsAbleToRecv()����̃��l�[��)

  Revision 1.10  2004/09/20 12:53:46  sato_masaki
  MBi_get_blockinfo()�̕Ԃ�l���ABOOL�l�ɕύX�B

  Revision 1.9  2004/09/14 06:34:21  sato_masaki
  MBDownloadFileInfo��reserved�̈��ǉ��B

  Revision 1.8  2004/09/07 04:33:46  sato_masaki
  MB_BlockInfo��segment_no��ǉ��B

  Revision 1.7  2004/09/06 09:20:01  miya
  ��`�폜

  Revision 1.6  2004/09/06 07:14:22  miya
  �d�l�ύX�ɂ��ύX

  Revision 1.5  2004/09/06 03:48:08  miya
  �F�؃��C�u�����ǉ�

  Revision 1.4  2004/09/06 02:36:00  miya
  �d�l�ύX�ɂ���`�ύX

  Revision 1.3  2004/09/04 11:03:00  sato_masaki
  ROM_HEADER_SIZE_FULL�̒�`��mb_fileinfo.c����ړ��B

  Revision 1.2  2004/09/04 09:56:32  sato_masaki
  �Z�O�����g�ADownloadFileInfo�Ɋւ����`��������ֈړ��B

  Revision 1.1  2004/09/03 07:06:28  sato_masaki
  �t�@�C�����@�\�ʂɕ����B


  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#ifndef	__MB_FILEINFO_H__
#define	__MB_FILEINFO_H__

#include <nitro.h>
#include <nitro/mb.h>

/* macro definition -------------------------------------------------------- */

#define MB_DL_SEGMENT_NUM				(3)     //      RomHeader, ARM9Static, ARM7Static��3�Z�O�����g
#define MB_DOWNLOAD_FILEINFO_SIZE		(sizeof(MBDownloadFileInfo))

#define MB_AUTHCODE_SIZE				(136)   /* Authentication code size */

#define ROM_HEADER_SIZE_FULL			(0x160)

/* type structure definition --------------------------------------------------------- */

//---------------------------------------------------------
// �e�@����q�@�ɑ��M����}���`�u�[�g�p�_�E�����[�h�t�@�C�����
//---------------------------------------------------------

/*
 * �Z�O�����g�^�C�v��`
 */
typedef enum MbSegmentType
{
    MB_SEG_ARM9STATIC = 0,
    MB_SEG_ARM7STATIC,
    MB_SEG_ROMHEADER
}
MbSegmentType;

/*
 * �}���`�u�[�g�����u�[�g�Z�O�����g���w�b�_�\����.
 */
typedef struct MbDownloadFileInfoHeader
{
    u32     arm9EntryAddr;             /* ARM9�G���g���A�h���X */
    u32     arm7EntryAddr;             /* ARM7�G���g���A�h���X */
    u32     padding;
}
MbDownloadFileInfoHeader;

/*
 * �Z�O�����g�\�����.
 */
typedef struct MbSegmentInfo
{
    u32     recv_addr;                 // ��M���̈ꎞ�i�[�A�h���X
    u32     load_addr;                 // ���[�h�A�h���X       �i���s�A�h���X�j
    u32     size;                      // �Z�O�����g�T�C�Y
    u32     target:1;                  // �^�[�Q�b�g           �iMI_PROCESSOR_ARM9 or _ARM7�B�������̃t�@�C����ARM9�Ƃ���B�j
    u32     rsv:31;                    // �\��B
}
MbSegmentInfo;                         // 16byte


/*
 * �����u�[�g�Z�O�����g�S�̂̍\����.
 */
typedef struct
{
    MbDownloadFileInfoHeader header;   // �w�b�_���(�G���g���[�A�h���X���i�[�����)
    MbSegmentInfo seg[MB_DL_SEGMENT_NUM];       // �Z�O�����g���
    u32     auth_code[MB_AUTHCODE_SIZE / sizeof(u32)];  // �F�ؗp�R�[�h
    u32     reserved[32 / sizeof(u32)]; // �\��̈�
}
MBDownloadFileInfo;


/* �u���b�N���Q�Ɨp�e�[�u�� */
typedef struct
{
    // �e�Z�O�����g�ɂ�����C���[�W�擪����̃I�t�Z�b�g
    u32     seg_src_offset[MB_DL_SEGMENT_NUM];
    u16     seg_head_blockno[MB_DL_SEGMENT_NUM];        // �Z�O�����g�̐擪�u���b�N�ԍ�
    u16     block_num;                 /* ���u���b�N�� */
}
MB_BlockInfoTable;


/* �u���b�N�]���֘A�f�[�^ */
typedef struct
{
    u32     child_address;             // �q�@�̊i�[�A�h���X
    u32     size;                      // Block�̃T�C�Y
    u32     offset;                    // �C���[�W�擪����̃I�t�Z�b�g�l�Ŏ��悤�ɂ���
    u8      segment_no;
    u8      pad[3];
}
MB_BlockInfo;

/* const valriables -------------------------------------------------------- */

extern const MbSegmentType MBi_defaultLoadSegList[MB_DL_SEGMENT_NUM];

/* functions --------------------------------------------------------------- */

/*
 * MbDownloadFileInfoHeader�ւ̃|�C���^����A
 * �Z�O�����g�����擾
 */
static inline MbSegmentInfo *MBi_GetSegmentInfo(MbDownloadFileInfoHeader * mdfi, int i)
{
    return ((MbSegmentInfo *) (mdfi + 1)) + i;
}

BOOL    MBi_MakeBlockInfoTable(MB_BlockInfoTable * table, MbDownloadFileInfoHeader * mdfi);
BOOL    MBi_get_blockinfo(MB_BlockInfo * bi, MB_BlockInfoTable * table, u32 block,
                          MbDownloadFileInfoHeader * mdfi);
u16     MBi_get_blocknum(MB_BlockInfoTable * table);
BOOL    MBi_IsAbleToRecv(u8 segment_no, u32 address, u32 size);

#endif /* __MB_FILEINFO_H__ */
