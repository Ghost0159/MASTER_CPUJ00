/*---------------------------------------------------------------------------*
  Project:  NitroSDK - MI - include
  File:     uncompress.h

  Copyright 2003-2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: uncompress.h,v $
  Revision 1.12  2006/01/18 02:12:29  kitase_hirotake
  do-indent

  Revision 1.11  2005/03/01 01:57:00  yosizaki
  copyright �̔N���C��.

  Revision 1.10  2005/02/28 05:26:02  yosizaki
  do-indent.

  Revision 1.9  2004/11/30 07:40:22  takano_makoto
  MICompressionHeader�\���̂�ǉ�

  Revision 1.8  2004/11/15 07:53:18  takano_makoto
  MI_UnfilterDiff8(), MI_UnfilterDiff16()��ǉ�

  Revision 1.7  2004/09/04 12:40:36  yasu
  Change interface between compstatic tool

  Revision 1.6  2004/09/02 09:48:37  yasu
  Add MIi_UncompressModule

  Revision 1.5  2004/07/20 07:28:42  yada
  some uncompressing functions are changed in system call functions renewal

  Revision 1.4  2004/06/18 01:28:48  yada
  add MI_GetUncompressedSize(), MI_GetCompressionType()

  Revision 1.3  2004/04/07 02:03:17  yada
  fix header comment

  Revision 1.2  2004/02/10 06:38:55  yada
  �֐�����ύX�BUnComp��UnCompress���B

  Revision 1.1  2004/02/10 01:20:25  yada
  UTL_ ����̈ڍs


  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#ifndef NITRO_MI_UNCOMPRESS_H_
#define NITRO_MI_UNCOMPRESS_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <nitro/misc.h>
#include <nitro/types.h>


//---- compression type
typedef enum
{
    MI_COMPRESSION_LZ = 0x10,          // LZ77
    MI_COMPRESSION_HUFFMAN = 0x20,     // Huffman
    MI_COMPRESSION_RL = 0x30,          // Run Length
    MI_COMPRESSION_DIFF = 0x80,        // Differential filter

    MI_COMPRESSION_TYPE_MASK = 0xf0,
    MI_COMPRESSION_TYPE_EX_MASK = 0xff
}
MICompressionType;


//----------------------------------------------------------------
// ���k�f�[�^�w�b�_�[
//
typedef struct
{
    u32     compParam:4;
    u32     compType:4;
    u32     destSize:24;

}
MICompressionHeader;



//----------------------------------------------------------------
// Bit���k�f�[�^�W�J�p�p�����[�^
//
typedef struct
{
    u16     srcNum;                    // �\�[�X�f�[�^�E�o�C�g��
    u16     srcBitNum:8;               // �P�\�[�X�f�[�^�E�r�b�g��
    u16     destBitNum:8;              // �P�f�X�e�B�l�[�V�����f�[�^�E�r�b�g��
    u32     destOffset:31;             // �\�[�X�f�[�^�ɉ��Z���鐔
    u32     destOffset0_on:1;          // �O�̃f�[�^�ɃI�t�Z�b�g�����Z���邩�ۂ��̃t���O
}
MIUnpackBitsParam;


//======================================================================
//          ���k�f�[�^�W�J
//======================================================================

//----------------------------------------------------------------------
//          �a�������k�f�[�^�W�J
//
//�E0�Œ�̃r�b�g���l�߂��f�[�^��W�J���܂��B
//�E�f�X�e�B�l�[�V�����A�h���X��4Byte���E�ɍ��킹�ĉ������B
//
//�E�����F
//             void *srcp     �\�[�X�A�h���X
//             void *destp    �f�X�e�B�l�[�V�����A�h���X
//  MIUnpackBitsParam *paramp   MIUnpackBitsParam�\���̂̃A�h���X
//
//�EMIUnpackBitsParam�\����
//    u16 srcNum              �\�[�X�f�[�^�E�o�C�g��
//    u8  srcBitNum           �P�\�[�X�f�[�^�E�r�b�g��
//    u8  destBitNum          �P�f�X�e�B�l�[�V�����f�[�^�E�r�b�g��
//    u32 destOffset:31       �\�[�X�f�[�^�ɉ��Z����I�t�Z�b�g��
//        destOffset0_On:1    �O�̃f�[�^�ɃI�t�Z�b�g�����Z���邩�ۂ��̃t���O
//
//�E�߂�l�F�Ȃ�
//----------------------------------------------------------------------

void    MI_UnpackBits(const void *srcp, void *destp, MIUnpackBitsParam *paramp);


//----------------------------------------------------------------------
//          �k�y�V�V���k�f�[�^�W�������W�J
//
//�ELZ77���k�f�[�^��W�J���A8bit�P�ʂŏ������݂܂��B
//�EVRAM�ɒ��ړW�J���邱�Ƃ͂ł��܂���B
//�E���k�f�[�^�̃T�C�Y��4�̔{���ɂȂ�Ȃ������ꍇ��
//  �o���邾��0�ŋl�߂Ē������ĉ������B
//�E�\�[�X�A�h���X��4Byte���E�ɍ��킹�ĉ������B
//
//�E�����F
//    void *srcp              �\�[�X�A�h���X
//    void *destp             �f�X�e�B�l�[�V�����A�h���X
//
//�E�f�[�^�w�b�_
//    u32 :4                  �\��
//        compType:4          ���k�^�C�v�i = 1�j
//        destSize:24         �W�J��̃f�[�^�T�C�Y
//
//�E�t���O�f�[�^�t�H�[�}�b�g
//    u8  flags               ���k�^�����k�t���O
//                            �i0, 1�j = �i�����k�f�[�^, ���k�f�[�^�j
//�E�R�[�h�f�[�^�t�H�[�}�b�g�iBig Endian�j
//    u16 length:4            �W�J�f�[�^�� - 3�i��v��3Byte�ȏ㎞�݈̂��k�j
//        offset:12           ��v�f�[�^�I�t�Z�b�g - 1
//
//�E�߂�l�F�Ȃ�
//----------------------------------------------------------------------

void    MI_UncompressLZ8(const void *srcp, void *destp);


//----------------------------------------------------------------------
//          �k�y�V�V���k�f�[�^�P�U�������W�J
//
//�ELZ77���k�f�[�^��W�J���A16bit�P�ʂŏ������݂܂��B
//�E�f�[�^TCM�⃁�C���������ɂ��W�J�ł��܂����AMI_UncompressLZ778()
//  ���ᑬ�ł��B
//�E���k�f�[�^�͈�v�������2Byte�ȑO��茟���������̂ɂ��ĉ������B
//�E���k�f�[�^�̃T�C�Y��4�̔{���ɂȂ�Ȃ������ꍇ��
//  �o���邾��0�ŋl�߂Ē������ĉ������B
//�E�\�[�X�A�h���X��4Byte���E�ɍ��킹�ĉ������B
//
//�E�����F
//    void *srcp              �\�[�X�A�h���X
//    void *destp             �f�X�e�B�l�[�V�����A�h���X
//
//�E�f�[�^�w�b�_
//    u32 :4                  �\��
//        compType:4          ���k�^�C�v�i = 1�j
//        destSize:24         �W�J��̃f�[�^�T�C�Y
//
//�E�t���O�f�[�^�t�H�[�}�b�g
//    u8  flags               ���k�^�����k�t���O
//                            �i0, 1�j = �i�����k�f�[�^, ���k�f�[�^�j
//�E�R�[�h�f�[�^�t�H�[�}�b�g�iBig Endian�j
//    u16 length:4            �W�J�f�[�^�� - 3�i��v��3Byte�ȏ㎞�݈̂��k�j
//        offset:12           ��v�f�[�^�I�t�Z�b�g�i >= 2�j - 1
//
//�E�߂�l�F�Ȃ�
//----------------------------------------------------------------------

void    MI_UncompressLZ16(const void *srcp, void *destp);


//----------------------------------------------------------------------
//          �n�t�}�����k�f�[�^�W�J
//
//�E�n�t�}�����k�f�[�^��W�J���A32bit�P�ʂŏ������݂܂��B
//�E���k�f�[�^�̃T�C�Y��4�̔{���ɂȂ�Ȃ������ꍇ��
//  �o���邾��0�ŋl�߂Ē������ĉ������B
//�E�\�[�X�A�h���X��4Byte���E�ɍ��킹�ĉ������B
//
//�E�����F
//    void *srcp              �\�[�X�A�h���X
//    void *destp             �f�X�e�B�l�[�V�����A�h���X
//
//�E�f�[�^�w�b�_
//    u32 bitSize:4           �P�f�[�^�E�r�b�g�T�C�Y�i�ʏ� 4|8�j
//        compType:4          ���k�^�C�v�i = 2�j
//        destSize:24         �W�J��̃f�[�^�T�C�Y
//
//�E�c���[�e�[�u��
//    u8           treeSize        �c���[�e�[�u���T�C�Y/2 - 1
//    TreeNodeData nodeRoot        ���[�g�m�[�h
//
//    TreeNodeData nodeLeft        ���[�g���m�[�h
//    TreeNodeData nodeRight       ���[�g�E�m�[�h
//
//    TreeNodeData nodeLeftLeft    �����m�[�h
//    TreeNodeData nodeLeftRight   ���E�m�[�h
//
//    TreeNodeData nodeRightLeft   �E���m�[�h
//    TreeNodeData nodeRightRight  �E�E�m�[�h
//
//            �E
//            �E
//
//  ���̌�Ɉ��k�f�[�^�{��
//
//�ETreeNodeData�\����
//    u8  nodeNextOffset:6    ���m�[�h�f�[�^�ւ̃I�t�Z�b�g - 1�i2Byte�P�ʁj
//        rightEndFlag:1      �E�m�[�h�I���t���O
//        leftEndzflag:1       ���m�[�h�I���t���O
//                            �I���t���O���Z�b�g����Ă���ꍇ
//                            ���m�[�h�Ƀf�[�^������
//
//�E�߂�l�F�Ȃ�
//----------------------------------------------------------------------

void    MI_UncompressHuffman(const void *srcp, void *destp);


//----------------------------------------------------------------------
//          ���������O�X���k�f�[�^�W�������W�J
//
//�E���������O�X���k�f�[�^��W�J���A8bit�P�ʂŏ������݂܂��B
//�EVRAM�ɒ��ړW�J���邱�Ƃ͂ł��܂���B
//�E���k�f�[�^�̃T�C�Y��4�̔{���ɂȂ�Ȃ������ꍇ��
//  �o���邾��0�ŋl�߂Ē������ĉ������B
//�E�\�[�X�A�h���X��4Byte���E�ɍ��킹�ĉ������B
//
//�E�����F
//    void *srcp              �\�[�X�A�h���X
//    void *destp             �f�X�e�B�l�[�V�����A�h���X
//
//�E�f�[�^�w�b�_
//    u32 :4                  �\��
//        compType:4          ���k�^�C�v�i = 3�j
//        destSize:24         �W�J��̃f�[�^�T�C�Y
//
//�E�t���O�f�[�^�t�H�[�}�b�g
//    u8  length:7            �W�J�f�[�^�� - 1�i�����k���j
//                            �W�J�f�[�^�� - 3�i�A����3Byte�ȏ㎞�݈̂��k�j
//        flag:1              �i0, 1�j = �i�����k�f�[�^, ���k�f�[�^�j
//
//�E�߂�l�F�Ȃ�
//----------------------------------------------------------------------

void    MI_UncompressRL8(const void *srcp, void *destp);


//----------------------------------------------------------------------
//          ���������O�X���k�f�[�^�P�U�������W�J
//
//�E���������O�X���k�f�[�^��W�J���A16bit�P�ʂŏ������݂܂��B
//�E�f�[�^TCM�⃁�C���������ɂ��W�J�ł��܂����AMI_UncompressRL8()
//  ���ᑬ�ł��B
//�E���k�f�[�^�̃T�C�Y��4�̔{���ɂȂ�Ȃ������ꍇ��
//  �o���邾��0�ŋl�߂Ē������ĉ������B
//�E�\�[�X�A�h���X��4Byte���E�ɍ��킹�ĉ������B
//
//�E�����F
//    void *srcp              �\�[�X�A�h���X
//    void *destp             �f�X�e�B�l�[�V�����A�h���X
//
//�E�f�[�^�w�b�_
//    u32 :4                  �\��
//        compType:4          ���k�^�C�v�i = 3�j
//        destSize:24         �W�J��̃f�[�^�T�C�Y
//
//�E�t���O�f�[�^�t�H�[�}�b�g
//    u8  length:7            �W�J�f�[�^�� - 1�i�����k���j
//                            �W�J�f�[�^�� - 3�i�A����3Byte�ȏ㎞�݈̂��k�j
//        flag:1              �i0, 1�j = �i�����k�f�[�^, ���k�f�[�^�j
//
//�E�߂�l�F�Ȃ�
//----------------------------------------------------------------------

void    MI_UncompressRL16(const void *srcp, void *destp);


//----------------------------------------------------------------------
//          �����t�B���^�ϊ��̕��� �W�������W�J
//
//�E�����t�B���^�𕜌����A8bit�P�ʂŏ������݂܂��B
//�EVRAM�ɒ��ړW�J���邱�Ƃ͂ł��܂���B
//�E���k�f�[�^�̃T�C�Y��4�̔{���ɂȂ�Ȃ������ꍇ��
//  �o���邾��0�ŋl�߂Ē������ĉ������B
//�E�\�[�X�A�h���X��4Byte���E�ɍ��킹�ĉ������B
//
//�E�����F
//    void *srcp              �\�[�X�A�h���X
//    void *destp             �f�X�e�B�l�[�V�����A�h���X
//
//�E�f�[�^�w�b�_
//    u32 :4                  �P�ʃr�b�g�T�C�Y
//        compType:4          ���k�^�C�v�i = 3�j
//        destSize:24         �W�J��̃f�[�^�T�C�Y
//
//�E�߂�l�F�Ȃ�
//----------------------------------------------------------------------

void    MI_UnfilterDiff8(const void *srcp, void *destp);

//----------------------------------------------------------------------
//          �����t�B���^�ϊ��̕��� �P�U�������W�J
//
//�E�����t�B���^�𕜌����A16bit�P�ʂŏ������݂܂��B
//�E�f�[�^TCM��VRAM�ɂ��W�J�ł��܂����AMI_Uncompress8()
//  ���ᑬ�ł��B//---- 
//�E���k�f�[�^�̃T�C�Y��4�̔{���ɂȂ�Ȃ������ꍇ��
//  �o���邾��0�ŋl�߂Ē������ĉ������B
//�E�\�[�X�A�h���X��4Byte���E�ɍ��킹�ĉ������B
//
//�E�����F
//    void *srcp              �\�[�X�A�h���X
//    void *destp             �f�X�e�B�l�[�V�����A�h���X
//
//�E�f�[�^�w�b�_
//    u32 :4                  �P�ʃr�b�g�T�C�Y
//        compType:4          ���k�^�C�v�i = 3�j
//        destSize:24         �W�J��̃f�[�^�T�C�Y
//
//�E�߂�l�F�Ȃ�
//----------------------------------------------------------------------

void    MI_UnfilterDiff16(const void *srcp, void *destp);


//================================================================================
/*---------------------------------------------------------------------------*
  Name:         MI_GetUncompressedSize

  Description:  get data size after uncompressing.
                this function can be used for all compress type
                (LZ8, LZ16, Huffman, RL8, RL16)

  Arguments:    srcp :  compressed data address

  Returns:      size
 *---------------------------------------------------------------------------*/
static inline u32 MI_GetUncompressedSize(const void *srcp)
{
    return (*(u32 *)srcp >> 8);
}

/*---------------------------------------------------------------------------*
  Name:         MI_GetCompressionType

  Description:  get compression type from compressed data

  Arguments:    srcp :  compressed data address

  Returns:      compression type.
                MI_COMPREESION_LZ      : mean compressed by LZ77
                MI_COMPREESION_HUFFMAN : mean compressed by Huffman
                MI_COMPREESION_RL      : mean compressed by Run Length
                MI_COMPRESSION_DIFF    : mean converted by Differential filter
 *---------------------------------------------------------------------------*/
static inline MICompressionType MI_GetCompressionType(const void *srcp)
{
    return (MICompressionType)(*(u32 *)srcp & MI_COMPRESSION_TYPE_MASK);
}



/*---------------------------------------------------------------------------*
  Name:         MIi_UncompressBackward

  Description:  Uncompress special archive for module compression

  Arguments:    bottom      = Bottom adrs of packed archive + 1
                bottom[-12] = offset for top    of compressed data
                                 inp_top = bottom + bottom[-12]
                bottom[ -8] = offset for bottom of compressed data
                                 inp     = bottom + bottom[ -8]
                bottom[ -4] = offset for bottom of original data
                                 outp    = bottom + bottom[ -4]
  
                typedef struct
                {
                    int         bufferTop;
                    int         compressBottom;
                    int         originalBottom;
                }   CompFooter;

  Returns:      None.
 *---------------------------------------------------------------------------*/
// !!!! Coded in libraries/init/ARM9/crt0.c
void    MIi_UncompressBackward(void *bottom);

#ifdef __cplusplus
} /* extern "C" */
#endif

/* NITRO_MI_UNCOMPRESS_H_ */
#endif
