/*---------------------------------------------------------------------------*
  Project:  NitroSDK - MI - include
  File:     uncomp_stream.h

  Copyright 2003-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: uncomp_stream.h,v $
  Revision 1.9  2007/11/12 09:46:58  seiki_masashi
  fix copyright header.

  Revision 1.8  2007/11/02 00:45:34  takano_makoto
  LZ77�g�����k�ɑΉ�

  Revision 1.7  2006/01/18 02:12:29  kitase_hirotake
  do-indent

  Revision 1.6  2005/03/01 01:57:00  yosizaki
  copyright �̔N���C��.

  Revision 1.5  2005/02/28 05:26:02  yosizaki
  do-indent.

  Revision 1.4  2004/12/01 00:03:12  takano_makoto
  �֐����� MI_UncompReadXX -> MI_ReadUncompXX�֕ύX

  Revision 1.3  2004/11/30 10:51:52  takano_makoto
  16bit�A�N�Z�X�֐���p��

  Revision 1.1  2004/11/30 04:18:26  takano_makoto
  Initial update.

  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#ifndef NITRO_MI_UNCOMP_STREAM_H_
#define NITRO_MI_UNCOMP_STREAM_H_

#include <nitro/types.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
    u8     *destp;                     // �������ݐ�|�C���^          4B
    s32     destCount;                 // �c�菑�����݃T�C�Y          4B
    u16     destTmp;                   // �������ݓr���̃f�[�^        2B
    u8      destTmpCnt;                // �������ݓr���̃f�[�^�T�C�Y  1B
    u8      flags;                     // ���k�t���O                  1B
    u16     length;                    // �A���������ݎc��T�C�Y      2B
    u8      _padding[2];               //                             2B
    //                         �v 16B
}
MIUncompContextRL;


typedef struct
{
    u8     *destp;                     // �������ݐ�|�C���^              4B
    s32     destCount;                 // �c�菑�����݃T�C�Y              4B
    u32     length;                    // �A���������ݎc�蒷              4B
    u16     destTmp;                   // �������ݓr���̃f�[�^            2B
    u8      destTmpCnt;                // �������ݓr���̃f�[�^�T�C�Y      1B
    u8      flags;                     // ���k�t���O                      1B
    u8      flagIndex;                 // �J�����g���k�t���O�C���f�b�N�X  1B
    u8      lengthFlg;                 // length�擾�σt���O              1B
    u8      exFormat;                  // LZ77���k�g���I�v�V����          1B
    u8      _padding[1];               //                                 1B
    //                             �v 20B
}
MIUncompContextLZ;


typedef struct
{
    u8     *destp;                     // �������ݐ�|�C���^                     4B
    s32     destCount;                 // �c�菑�����݃T�C�Y                     4B
    u8     *treep;                     // �n�t�}�������e�[�u���A�J�����g�|�C���^ 4B
    u32     srcTmp;                    // �ǂݍ��ݓr���̃f�[�^                   4B
    u32     destTmp;                   // �������r���̃f�[�^                     4B
    s16     treeSize;                  // �n�t�}�������e�[�u���T�C�Y             2B
    u8      srcTmpCnt;                 // �ǂݍ��ݓr���̃f�[�^�T�C�Y             1B
    u8      destTmpCnt;                // �������ς̃r�b�g��                     1B
    u8      bitSize;                   // �������r�b�g�T�C�Y                     1B
    u8      _padding2[3];              //                                        3B
    u8      tree[0x200];               // �n�t�}�������e�[�u��                 512B  (4bit�������ł����32B�ŏ\���Ȃ̂ł���8bit���m��)
    //                                   �v 540B  (4bit�������̏ꍇ��60B����΂悢)
}
MIUncompContextHuffman;


void    MI_InitUncompContextRL(MIUncompContextRL *context, u8 *dest,
                               const MICompressionHeader *header);
void    MI_InitUncompContextLZ(MIUncompContextLZ *context, u8 *dest,
                               const MICompressionHeader *header);
void    MI_InitUncompContextHuffman(MIUncompContextHuffman *context, u8 *dest,
                                    const MICompressionHeader *header);

s32     MI_ReadUncompRL8(MIUncompContextRL *context, const u8 *data, u32 len);
s32     MI_ReadUncompRL16(MIUncompContextRL *context, const u8 *data, u32 len);
s32     MI_ReadUncompLZ8(MIUncompContextLZ *context, const u8 *data, u32 len);
s32     MI_ReadUncompLZ16(MIUncompContextLZ *context, const u8 *data, u32 len);
s32     MI_ReadUncompHuffman(MIUncompContextHuffman *context, const u8 *data, u32 len);

#ifdef __cplusplus
} /* extern "C" */
#endif

/* NITRO_MI_UNCOMP_STREAM_H_ */
#endif
