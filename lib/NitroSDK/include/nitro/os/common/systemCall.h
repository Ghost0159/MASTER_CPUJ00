/*---------------------------------------------------------------------------*
  Project:  NitroSDK - OS - include
  File:     systemCall.h

  Copyright 2003-2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: systemCall.h,v $
  Revision 1.24  2006/03/13 05:59:37  okubata_ryoma
  �}�N���̔��C��

  Revision 1.23  2006/01/18 02:12:28  kitase_hirotake
  do-indent

  Revision 1.22  2005/11/21 02:25:10  terui
  Reconstruct SVC_WaitVBlankIntr definition.

  Revision 1.21  2005/10/24 00:49:42  adachi_hiroaki
  SVC_WaitVBlankIntr()��SDK_WEAK_SYMBOL�w����O����

  Revision 1.20  2005/10/21 09:00:02  yada
  let SVC_WaitVBlankIntr be weak symbol.

  Revision 1.19  2005/03/01 01:57:00  yosizaki
  copyright �̔N���C��.

  Revision 1.18  2005/02/28 05:26:01  yosizaki
  do-indent.

  Revision 1.17  2004/11/02 09:48:48  yada
  just fix comment

  Revision 1.16  2004/07/20 07:27:11  yada
  system call functions were renewed

  Revision 1.15  2004/05/26 11:10:02  terui
  change comment of SVC_WaitByLoop

  Revision 1.14  2004/04/15 11:52:27  yada
  delete SVC_ReadCard

  Revision 1.13  2004/04/07 02:03:17  yada
  fix header comment

  Revision 1.12  2004/03/26 13:11:31  yasu
  #if SDK_ARM7 -> #ifdef SDK_ARM7

  Revision 1.11  2004/03/25 09:43:20  yada
  ARM7�� SVC_Halt(), SVC_WaitByLoop() �ǉ�

  Revision 1.10  2004/03/17 10:36:50  yasu
  fix SVC_CpuCopyFast

  Revision 1.9  2004/02/10 11:34:05  yada
  SystemCall() ���폜

  Revision 1.8  2004/02/05 07:27:19  yada
  ���𕶎����IRIS���������̂� NITRO ���� IRIS�ɖ߂����B

  Revision 1.7  2004/02/05 07:09:03  yasu
  change SDK prefix iris -> nitro

  Revision 1.6  2004/02/05 00:19:53  yada
  #define �ŁA{ } �݂̂ň͂��Ă�����̂� do{ }while(0) �ɂ����B

  Revision 1.5  2004/01/19 06:14:09  yada
  SVC_CpuSet() �����̒萔��MI_ �����Ă��Ȃ������������̂��C��

  Revision 1.4  2003/12/11 00:35:20  yasu
  IRIS_TS -> SDK_TS �Ȃǂ̏C��

  Revision 1.3  2003/11/28 01:54:54  yada
  REDSDK��03-11-27���f

  Revision 1.2  2003/11/11 12:27:28  yada
  �V�X�e���R�[�������������� interrupt.h ���� systemCall.h �Ɉڂ����B

  Revision 1.1  2003/11/07 07:55:16  yada
  �b��łł��B�����Ƒ傫���ύX�ɂȂ�܂��B


  $NoKeywords: $

 *---------------------------------------------------------------------------*/

#ifndef NITRO_OS_SYSTEMCALL_H_
#define NITRO_OS_SYSTEMCALL_H_

#include <nitro/mi/stream.h>
#include <nitro/mi/uncompress.h>
#include <nitro/os/common/system.h>

#ifdef __cplusplus
extern "C" {
#endif


//================================================================================
/*---------------------------------------------------------------------------*
  Name:         SVC_WaitVBlankIntr

  Description:  wait for VBlank

              �EV�u�����N���荞�݂���������܂Ńz�[���g��Ԃő҂������܂��B
              �E���荞�ݏ����ɂ�INTR_CHECK_BUF (DTCM_END - 0x8) �֊Y��
                ����t���O���Z�b�g���ĉ������B
              �E�����̊��荞�݂𕹗p�������ASVC_Halt()���J��Ԃ��Ăяo�����
                �ꍇ�Ɣ�ׂăV�X�e���R�[���Ăяo���̃I�[�o�[�w�b�h���y������
                ���Ƃ��ł��܂��B
              �E�X���b�h�g�p���Ƀz�[���g�֓���܂��ƁA���荞�݂���������܂�
                ���̃X���b�h���~�܂��Ă��܂����ƂɂȂ�܂��̂ŁA�A�C�h���X���b�h
                �ȊO��OS_WaitIrq(1, OS_IE_V_BLANK)�̎g�p�𐄏����܂��B

  Arguments:    None

  Returns:      None

 *---------------------------------------------------------------------------*/
#ifdef  SDK_SVC_WAITVBLANK_COMPATIBLE
void    SVC_WaitVBlankIntr(void);
#else
#define SVC_WaitVBlankIntr  OS_WaitVBlankIntr
#endif

/*---------------------------------------------------------------------------*
  Name:         SVC_WaitByLoop

  Description:  loop specified times

              �E�V�X�e��ROM��Ŏw��񐔃��[�v�������s���܂��B
              �E1��̃��[�v��4�T�C�N��������܂��B
              �E�ʏ펞�̃T�u�v���Z�b�T�����C���������ւ̗D�挠�������Ă����Ԃɂ����ẮA
                �T�u�v���Z�b�T���V�X�e��ROM��Ńv���O�����𓮍삳���邱�Ƃɂ����
                ���C���v���Z�b�T���X�g�[������̂��y�����邱�Ƃ��ł��܂��B
              �E���C���������\�����[�h�Ȃǃ��C���v���Z�b�T�֗D�挠��^����
                �K�v������ꍇ�ɂ́A���C���v���Z�b�T���ŌĂяo�����Ƃ�
                �L���ȏ󋵂����邩������܂��񂪁A�قƂ�ǂ̊��Ԃ��L���b�V����
                TCM��œ��삵�Ă���΃��C���v���Z�b�T����Ăяo���K�v�͂���܂���B

              �E�����F
                  count  :     ���[�v��

  Arguments:    count : times to loop 

  Returns:      None

 *---------------------------------------------------------------------------*/
void    SVC_WaitByLoop(s32 count);

/*---------------------------------------------------------------------------*
  Name:         SVC_CpuSet

  Description:  clear or copy memory by cpu

               �EDmaSet�}�N���݊��̃p�����[�^��RAM�N���A�܂��̓R�s�[���܂��B
               �E32bit�]���ł͋����I��4Byte���E�ɂăA�N�Z�X����܂����A
                 16bit�]���ł͈�����2Byte���E�֍��킹�ēn���K�v������܂��B

               �E�����F
                   srcp          �\�[�X�A�h���X
                   destp         �f�X�e�B�l�[�V�����A�h���X
                   dmaCntData    MI_DMA_SRC_FIX�^MI_DMA_32BIT_BUS�^MI_DMA_COUNT_MASK�̂ݗL��

                         MI_DMA_SRC_FIX(  0, 1) = (�\�[�X�A�h���X�E�C���N�������g, �\�[�X�A�h���X�Œ�)
                         MI_DMA_32BIT_BUS(0, 1) = (16bit�]��, 32bit�]��)
                         MI_DMA_COUNT_MASK & dmaCntData = �]����

               �E��ʃ}�N���F
                   SVC_CpuClear, SVC_CpuClearArray, SVC_CpuCopy, SVC_CpuCopyArray

  Arguments:    srcp :       source address
                destp :      destination addrss
                dmaCntData : dma control data
                            (only MI_DMA_SRC_FIX/MI_DMA_32BIT_BUS/MI_DMA_COUNT_MASK
                             is available)

  Returns:      None

 *---------------------------------------------------------------------------*/
void    SVC_CpuSet(const void *srcp, void *destp, u32 dmaCntData);

/*---------------------------------------------------------------------------*
  Name:         SVC_CpuSetFast

  Description:  clear or copy memory by cpu quickly

              �EDmaSet�}�N���݊��̃p�����[�^�ō�����RAM�N���A�܂��̓R�s�[���܂��B
              �E32Byte�P�ʂŃA�N�Z�X�\�Ȏ��ɂ͕������[�h�^�X�g�A���߂��g�p����A
                �[����4Byte�P�ʂŃA�N�Z�X����܂��B
              �E4Byte���E�ȊO�ň�����^���Ă������I��4Byte���E�ł̃A�N�Z�X�ɂȂ�܂��B

              �E�����F
                  srcp          �\�[�X�A�h���X
                  destp         �f�X�e�B�l�[�V�����A�h���X
                  dmaCntData     MI_DMA_SRC_FIX�^MI_DMA_COUNT_MASK�̂ݗL��

                        MI_DMA_SRC_FIX(0, 1) = (�\�[�X�A�h���X�E�C���N�������g, �\�[�X�A�h���X�Œ�)
                        MI_DMA_COUNT_MASK & dmaCntData = �]����

              �E��ʃ}�N���F
                  SVC_CpuClearFast, SVC_CpuClearArrayFast, SVC_CpuCopyFast, SVC_CpuCopyArrayFast

  Arguments:    srcp :       source address
                destp :      destination addrss
                dmaCntData : dma control data
                            (only MI_DMA_SRC_FIX/MI_DMA_COUNT_MASK
                             is available)

  Returns:      None

 *---------------------------------------------------------------------------*/
void    SVC_CpuSetFast(const void *srcp, void *destp, u32 dmaCntData);

/*---------------------------------------------------------------------------*
  Name:         SVC_CpuClear

  Description:  clear memory by SVC_CpuSet

              �ECPU��RAM�N���A����V�X�e���R�[�����Ăяo���܂��B
              �E�N���A�f�[�^�̓X�^�b�N�ɒu����A������f�X�e�B�l�[�V�����փR�s�[���܂��B

              �E�����F
                  data        �N���A�f�[�^
                  destp       �f�X�e�B�l�[�V�����A�h���X
                  size        �N���A�o�C�g��
                  bit         �]���r�b�g���i16|32�j

  Arguments:    data  : clear data
                destp : destination address
                size  : clear size ( by byte )
                bit   : bit width ( 16 or 32 )

  Returns:      None

 *---------------------------------------------------------------------------*/
#define SVC_CpuClear( data, destp, size, bit )                  \
do{                                                             \
    vu##bit tmp = (vu##bit )(data);                             \
    SVC_CpuSet((u8 *)&(tmp), (u8 *)(destp), (                   \
        MI_DMA_SRC_FIX        |                                 \
        MI_DMA_##bit##BIT_BUS | ((size)/((bit)/8) & 0x1fffff)));  \
} while(0)

/*---------------------------------------------------------------------------*
  Name:         SVC_CpuClearArray

  Description:  clear memory by SVC_CpuSet

              �ECPU��RAM�N���A����V�X�e���R�[�����Ăяo���܂��B
              �E�N���A�f�[�^�̓X�^�b�N�ɒu����A������f�X�e�B�l�[�V�����փR�s�[���܂��B
              �ESVC_CpuClearArray�̓f�X�e�B�l�[�V�����z��S�̂��N���A���܂��B

              �E�����F
                  data        �N���A�f�[�^
                  destp       �f�X�e�B�l�[�V�����A�h���X
                  bit         �]���r�b�g���i16|32�j

  Arguments:    data  : clear data
                destp : destination address
                bit   : bit width ( 16 or 32 )

  Returns:      None

 *---------------------------------------------------------------------------*/
#define SVC_CpuClearArray( data, destp, bit )                   \
        SVC_CpuClear( data, destp, sizeof(destp), bit )

/*---------------------------------------------------------------------------*
  Name:         SVC_CpuCopy

  Description:  copy memory by SVC_CpuSet

              �ECPU�ŃR�s�[����V�X�e���R�[�����Ăяo���܂��B
              �ESVC_CpuCopyArray�̓\�[�X�z��S�̂��R�s�[���܂��B

              �E�����F
                  srcp      :  �\�[�X�A�h���X
                  destp     :  �f�X�e�B�l�[�V�����A�h���X
                  size      :  �]���o�C�g��
                  bit       :  �]���r�b�g���i16|32�j

  Arguments:    srcp      : source address
                destp     : destination address
                size      : size to copy ( by byte )
                bit       : bit width ( 16 or 32 )

  Returns:      None

 *---------------------------------------------------------------------------*/
#define SVC_CpuCopy( srcp, destp, size, bit )                   \
                                                                \
    SVC_CpuSet((u8 *)(srcp), (u8 *)(destp),  (                  \
        MI_DMA_SRC_INC        |                                 \
        MI_DMA_##bit##BIT_BUS | ((size)/((bit)/8) & 0x1fffff)))

/*---------------------------------------------------------------------------*
  Name:         SVC_CpuCopyArray

  Description:  copy memory by SVC_CpuSet

              �ECPU�ŃR�s�[����V�X�e���R�[�����Ăяo���܂��B
              �ESVC_CpuCopyArray�̓\�[�X�z��S�̂��R�s�[���܂��B

              �E�����F
                  srcp      :  �\�[�X�A�h���X
                  destp     :  �f�X�e�B�l�[�V�����A�h���X
                  bit       :  �]���r�b�g���i16|32�j

  Arguments:    srcp      : source address
                destp     : destination address
                bit       : bit width ( 16 or 32 )

  Returns:      None

 *---------------------------------------------------------------------------*/
#define SVC_CpuCopyArray( srcp, destp, bit  )                  \
        SVC_CpuCopy( srcp, destp, sizeof(srcp), bit )

/*---------------------------------------------------------------------------*
  Name:         SVC_CpuClearFast

  Description:  clear memory by SVC_CpuSetFast quickly

              �ECPU�ō�����RAM�N���A����V�X�e���R�[�����Ăяo���܂��B
              �E�N���A�f�[�^�̓X�^�b�N�ɒu����A������f�X�e�B�l�[�V�����փR�s�[���܂��B
              �E32Byte�P�ʂŃA�N�Z�X�\�Ȏ��ɂ�32Byte�P�ʂ̕����X�g�A���߂��g�p����A
                �[����4Byte�P�ʂŃA�N�Z�X����܂��B

              �E�����F
                  data        �N���A�f�[�^
                  destp       �f�X�e�B�l�[�V�����A�h���X
                  size        �N���A�o�C�g��

  Arguments:    data  : clear data
                destp : destination address
                size  : clear size ( by byte )

  Returns:      None

 *---------------------------------------------------------------------------*/
#define SVC_CpuClearFast( data, destp, size )                   \
do{                                                             \
    vu32 tmp = (vu32 )(data);                                   \
    SVC_CpuSetFast((u8 *)&(tmp), (u8 *)(destp), (               \
        MI_DMA_SRC_FIX | ((size)/(32/8) & 0x1fffff)));   \
} while(0)

/*---------------------------------------------------------------------------*
  Name:         SVC_CpuClearArrayFast

  Description:  clear memory by SVC_CpuSetFast quickly

              �ECPU�ō�����RAM�N���A����V�X�e���R�[�����Ăяo���܂��B
              �E�N���A�f�[�^�̓X�^�b�N�ɒu����A������f�X�e�B�l�[�V�����փR�s�[���܂��B
              �E32Byte�P�ʂŃA�N�Z�X�\�Ȏ��ɂ͕����X�g�A���߂��g�p����A
                �[����4Byte�P�ʂŃA�N�Z�X����܂��B
              �ESVC_CpuClearArrayFast�̓f�X�e�B�l�[�V�����z��S�̂��N���A���܂��B

              �E�����F
                  data        �N���A�f�[�^
                  destp       �f�X�e�B�l�[�V�����A�h���X

  Arguments:    data  : clear data
                size  : size to clear ( by byte )

  Returns:      None
 *---------------------------------------------------------------------------*/
#define SVC_CpuClearArrayFast( data, destp )                    \
        SVC_CpuClearFast( data, destp, sizeof(destp) )

/*---------------------------------------------------------------------------*
  Name:         SVC_CpuCopyFast

  Description:  clear memory by SVC_CpuSetFast quickly

              �ECPU�ō����ɃR�s�[����V�X�e���R�[�����Ăяo���܂��B
              �E32Byte�P�ʂŃA�N�Z�X�\�Ȏ��ɂ͕������[�h�^�X�g�A���߂��g�p����A
                �[����4Byte�P�ʂŃA�N�Z�X����܂��B

              �E�����F
                  srcp        �\�[�X�A�h���X
                  destp       �f�X�e�B�l�[�V�����A�h���X
                  size        �]���o�C�g��

  Arguments:    srcp  : source address
                destp : destination address
                size  : size to copy ( by byte )

  Returns:      None

 *---------------------------------------------------------------------------*/
#define SVC_CpuCopyFast( srcp, destp, size )                    \
                                                                \
    SVC_CpuSetFast((u8 *)(srcp), (u8 *)(destp),  (              \
        MI_DMA_SRC_INC | ((size)/(32/8) & 0x1fffff)))

/*---------------------------------------------------------------------------*
  Name:         SVC_CpuCopyArrayFast

  Description:  clear memory by SVC_CpuSetFast quickly

              �ECPU�ō����ɃR�s�[����V�X�e���R�[�����Ăяo���܂��B
              �E32Byte�P�ʂŃA�N�Z�X�\�Ȏ��ɂ͕������[�h�^�X�g�A���߂��g�p����A
                �[����4Byte�P�ʂŃA�N�Z�X����܂��B
              �ESVC_CpuCopyArrayFast�̓\�[�X�z��S�̂��R�s�[���܂��B

              �E�����F
                  srcp        �\�[�X�A�h���X
                  destp       �f�X�e�B�l�[�V�����A�h���X

  Arguments:    srcp  : source address
                destp : destination address

  Returns:      None

 *---------------------------------------------------------------------------*/
#define SVC_CpuCopyArrayFast( srcp, destp )                     \
        SVC_CpuCopyFast( srcp, destp, sizeof(srcp) )

/*---------------------------------------------------------------------------*
  Name:         SVC_UnpackBits

  Description:  unpack bits

              �E0�Œ��bit���l�߂��f�[�^��W�J���܂��B
              �E�f�X�e�B�l�[�V�����A�h���X��4Byte���E�ɍ��킹�ĉ������B

              �E�����F
                   srcp          �\�[�X�A�h���X
                   destp         �f�X�e�B�l�[�V�����A�h���X
                   paramp        MIUnpackBitsParam�\���̂̃A�h���X

              �EMIUnpackBitsParam�\����
                    u16 srcNum              �\�[�X�f�[�^�E�o�C�g��
                    u8  srcBitNum           �P�\�[�X�f�[�^�E�r�b�g��
                    u8  destBitNum          �P�f�X�e�B�l�[�V�����f�[�^�E�r�b�g��
                    u32 destOffset:31       �\�[�X�f�[�^�ɉ��Z����I�t�Z�b�g��
                        destOffset0_On:1    �O�̃f�[�^�ɃI�t�Z�b�g�����Z���邩�ۂ��̃t���O

  Arguments:    srcp   : source address
                destp  : destination address
                paramp : parameter structure address

  Returns:      None

 *---------------------------------------------------------------------------*/
void    SVC_UnpackBits(const void *srcp, void *destp, const MIUnpackBitsParam *paramp);

/*---------------------------------------------------------------------------*
  Name:         SVC_UncompressLZ8

  Description:  uncompress LZ77

              �ELZ77���k�f�[�^��W�J���A8bit�P�ʂŏ������݂܂��B
              �E�o�C�g�A�N�Z�X�ł��Ȃ�VRAM���ɒ��ړW�J���邱�Ƃ͂ł��܂���B
              �E���k�f�[�^�̃T�C�Y��4�̔{���ɂȂ�Ȃ������ꍇ��
                �o���邾��0�ŋl�߂Ē������ĉ������B
              �E�\�[�X�A�h���X��4Byte���E�ɍ��킹�ĉ������B

              �E�����F
                   srcp          �\�[�X�A�h���X
                   destp         �f�X�e�B�l�[�V�����A�h���X

               �E�f�[�^�w�b�_
                   u32 :4                  �\��
                        compType:4          ���k�^�C�v�i = 1�j
                        destSize:24         �W�J��̃f�[�^�T�C�Y

               �E�t���O�f�[�^�t�H�[�}�b�g
                    u8  flags               ���k�^�����k�t���O
                                            �i0, 1�j = �i�����k�f�[�^, ���k�f�[�^�j
               �E�R�[�h�f�[�^�t�H�[�}�b�g�iBig Endian�j
                    u16 length:4            �W�J�f�[�^�� - 3�i��v��3Byte�ȏ㎞�݈̂��k�j
                        offset:12           ��v�f�[�^�I�t�Z�b�g - 1

  Arguments:    srcp  : source address
                destp : destination address

  Returns:      None

 *---------------------------------------------------------------------------*/
void    SVC_UncompressLZ8(const void *srcp, void *destp);

/*---------------------------------------------------------------------------*
  Name:         SVC_UncompressRL8

  Description:  uncompress run length

              �E���������O�X���k�f�[�^��W�J���A8bit�P�ʂŏ������݂܂��B
              �E�o�C�g�A�N�Z�X�ł��Ȃ�VRAM���ɒ��ړW�J���邱�Ƃ͂ł��܂���B
              �E���k�f�[�^�̃T�C�Y��4�̔{���ɂȂ�Ȃ������ꍇ��
                �o���邾��0�ŋl�߂Ē������ĉ������B
              �E�\�[�X�A�h���X��4Byte���E�ɍ��킹�ĉ������B

              �E�����F
                   srcp          �\�[�X�A�h���X
                   destp         �f�X�e�B�l�[�V�����A�h���X

              �E�f�[�^�w�b�_
                   u32 :4                  �\��
                       compType:4          ���k�^�C�v�i = 3�j
                       destSize:24         �W�J��̃f�[�^�T�C�Y

              �E�t���O�f�[�^�t�H�[�}�b�g
                   u8  length:7            �W�J�f�[�^�� - 1�i�����k���j
                                           �W�J�f�[�^�� - 3�i�A����3Byte�ȏ㎞�݈̂��k�j
                       flag:1              �i0, 1�j = �i�����k�f�[�^, ���k�f�[�^�j

  Arguments:    srcp  : source address
                destp : destination address

  Returns:      None

 *---------------------------------------------------------------------------*/
void    SVC_UncompressRL8(const void *srcp, void *destp);

#ifdef SDK_TS
/*---------------------------------------------------------------------------*
  Name:         SVC_UncompressLZ16FromDevice

  Description:  uncompress LZ77 from device

              �ELZ77���k�f�[�^��W�J���A16bit�P�ʂŏ������݂܂��B
              �E�������}�b�s���O����Ă��Ȃ��f�o�C�X��̈��k�f�[�^��
                �e���|�����o�b�t�@���g�킸�ɒ��ړW�J���邱�Ƃ��ł��܂��B
              �E�o�C�g�A�N�Z�X�ł��Ȃ�RAM�ɂ��W�J�ł��܂����A
                SVC_UncompressLZ8()���ᑬ�ł��B
              �E���k�f�[�^�͈�v�������2Byte�ȑO��茟���������̂ɂ��ĉ������B
              �E���k�f�[�^�̃T�C�Y��4�̔{���ɂȂ�Ȃ������ꍇ��
                �o���邾��0�ŋl�߂Ē������ĉ������B
              �E�\�[�X�A�h���X��4Byte���E�ɍ��킹�ĉ������B
              �E�������͓W�J��̃T�C�Y��Ԃ��A���s���͕��̒l��Ԃ��܂��B
                initStream/terminateStream�R�[���o�b�N�֐����ŃG���[��
                ���o�����ꍇ�͕��̒l��Ԃ��ĉ������B

              �E�����F
                   srcp          �\�[�X�A�h���X
                   destp         �f�X�e�B�l�[�V�����A�h���X
                   paramp        MIReadStreamCallbacks�\���̂�initStream�֐��֓n���p�����[�^�̃A�h���X
                   callbacks     MIReadStreamCallbacks�\���̂̃A�h���X

               �E�f�[�^�w�b�_
                   u32 :4                  �\��
                       compType:4          ���k�^�C�v�i = 1�j
                       destSize:23         �W�J��̃f�[�^�T�C�Y
                       :1                  �g�p�s��

               �E�t���O�f�[�^�t�H�[�}�b�g
                    u8  flags               ���k�^�����k�t���O
                                            �i0, 1�j = �i�����k�f�[�^, ���k�f�[�^�j
               �E�R�[�h�f�[�^�t�H�[�}�b�g�iBig Endian�j
                    u16 length:4            �W�J�f�[�^�� - 3�i��v��3Byte�ȏ㎞�݈̂��k�j
                        offset:12           ��v�f�[�^�I�t�Z�b�g - 1

  Arguments:    srcp      : source address
                destp     : destination address
                callbacks : address of stream callbacks structure

  Returns:      uncompressed size >= 0
                error < 0

 *---------------------------------------------------------------------------*/
s32     SVC_UncompressLZ16FromDevice(const void *srcp, void *destp, const void *paramp,
                                     const MIReadStreamCallbacks *callbacks);

/*---------------------------------------------------------------------------*
  Name:         SVC_UncompressRL16FromDevice

  Description:  uncompress run length from device

              �E���������O�X���k�f�[�^��W�J���A16bit�P�ʂŏ������݂܂��B
              �E�������}�b�s���O����Ă��Ȃ��f�o�C�X��̈��k�f�[�^��
                �e���|�����o�b�t�@���g�킸�ɒ��ړW�J���邱�Ƃ��ł��܂��B
              �E�o�C�g�A�N�Z�X�ł��Ȃ�RAM�ɂ��W�J�ł��܂����A
                SVC_UncompressRL8()���ᑬ�ł��B
              �E���k�f�[�^�̃T�C�Y��4�̔{���ɂȂ�Ȃ������ꍇ��
                �o���邾��0�ŋl�߂Ē������ĉ������B
              �E�\�[�X�A�h���X��4Byte���E�ɍ��킹�ĉ������B
              �E�������͓W�J��̃T�C�Y��Ԃ��A���s���͕��̒l��Ԃ��܂��B
                initStream/terminateStream�R�[���o�b�N�֐����ŃG���[��
                ���o�����ꍇ�͕��̒l��Ԃ��ĉ������B

              �E�����F
                   srcp          �\�[�X�A�h���X
                   destp         �f�X�e�B�l�[�V�����A�h���X
                   paramp        MIReadStreamCallbacks�\���̂�initStream�֐��֓n���p�����[�^�̃A�h���X
                   callbacks     MIReadStreamCallbacks�\���̂̃A�h���X

              �E�f�[�^�w�b�_
                   u32 :4                  �\��
                       compType:4          ���k�^�C�v�i = 3�j
                       destSize:23         �W�J��̃f�[�^�T�C�Y
                       :1                  �g�p�s��

              �E�t���O�f�[�^�t�H�[�}�b�g
                   u8  length:7            �W�J�f�[�^�� - 1�i�����k���j
                                           �W�J�f�[�^�� - 3�i�A����3Byte�ȏ㎞�݈̂��k�j
                       flag:1              �i0, 1�j = �i�����k�f�[�^, ���k�f�[�^�j

  Arguments:    srcp      : source address
                destp     : destination address
                callbacks : address of stream callbacks structure

  Returns:      uncompressed size >= 0
                error < 0

 *---------------------------------------------------------------------------*/
s32     SVC_UncompressRL16FromDevice(const void *srcp, void *destp, const void *paramp,
                                     const MIReadStreamCallbacks *callbacks);

/*---------------------------------------------------------------------------*
  Name:         SVC_UncompressHuffmanFromDevice

  Description:  uncompress huffman from device

              �E�n�t�}�����k�f�[�^��W�J���A32bit�P�ʂŏ������݂܂��B
              �E�������}�b�s���O����Ă��Ȃ��f�o�C�X��̈��k�f�[�^��
                �e���|�����o�b�t�@���g�킸�ɒ��ړW�J���邱�Ƃ��ł��܂��B
              �E���k�f�[�^�̃T�C�Y��4�̔{���ɂȂ�Ȃ������ꍇ��
                �o���邾��0�ŋl�߂Ē������ĉ������B
              �E�\�[�X�A�h���X��4Byte���E�ɍ��킹�ĉ������B
              �E�������͓W�J��̃T�C�Y��Ԃ��A���s���͕��̒l��Ԃ��܂��B
                initStream/terminateStream�R�[���o�b�N�֐����ŃG���[��
                ���o�����ꍇ�͕��̒l��Ԃ��ĉ������B

              �E�����F
                   srcp          �\�[�X�A�h���X
                   destp         �f�X�e�B�l�[�V�����A�h���X
                   tableBufp     �c���[�e�[�u���i�[�o�b�t�@�i�ő�512Byte�j
                                 MIReadStreamCallbacks�\���̂�initStream�֐��փp�����[�^��n�������ꍇ�A
                                 ���̃o�b�t�@�o�R���ēn�����Ƃ��ł��܂��B
                                 �������AinitStream�֐��Ăяo����̓c���[�e�[�u���ŏ㏑������܂��B
                   callbacks     MIReadStreamCallbacks�\���̂̃A�h���X

              �E�f�[�^�w�b�_
                   u32 bitSize:4           �P�f�[�^�E�r�b�g�T�C�Y�i�ʏ� 4|8�j
                       compType:4          ���k�^�C�v�i = 2�j
                       destSize:23         �W�J��̃f�[�^�T�C�Y
                       :1                  �g�p�s��

              �E�c���[�e�[�u��
                   u8           treeSize        �c���[�e�[�u���T�C�Y/2 - 1
                   TreeNodeData nodeRoot        ���[�g�m�[�h

                   TreeNodeData nodeLeft        ���[�g���m�[�h
                   TreeNodeData nodeRight       ���[�g�E�m�[�h

                   TreeNodeData nodeLeftLeft    �����m�[�h
                   TreeNodeData nodeLeftRight   ���E�m�[�h

                   TreeNodeData nodeRightLeft   �E���m�[�h
                   TreeNodeData nodeRightRight  �E�E�m�[�h

                           �E
                           �E

                ���̌�Ɉ��k�f�[�^�{��

              �ETreeNodeData�\����
                  u8  nodeNextOffset:6    ���m�[�h�f�[�^�ւ̃I�t�Z�b�g - 1�i2Byte�P�ʁj
                      rightEndFlag:1      �E�m�[�h�I���t���O
                      leftEndflag:1       ���m�[�h�I���t���O
                                          �I���t���O���Z�b�g����Ă���ꍇ
                                          ���m�[�h�Ƀf�[�^������

  Arguments:    srcp      : source address
                destp     : destination address
                callbacks : address of stream callbacks structure

  Returns:      uncompressed size >= 0
                error < 0

 *---------------------------------------------------------------------------*/
s32     SVC_UncompressHuffmanFromDevice(const void *srcp, void *destp, u8 *tableBufp,
                                        const MIReadStreamCallbacks *callbacks);

/*---------------------------------------------------------------------------*
  Name:         SVC_GetCRC16

  Description:  calculate CRC-16

              �ECRC-16���Z�o���܂��B
              �E�f�[�^�A�h���X�ƃT�C�Y��2Byte���E�ɍ��킹�ĉ������B

              �E�����F
                   start         �����l
                   datap         �f�[�^�A�h���X
                   size          �T�C�Y�i�o�C�g���j

  Arguments:    start  : start value
                datap  : data address
                size   : data size (by byte)

  Returns:      CRC-16

 *---------------------------------------------------------------------------*/
u16     SVC_GetCRC16(u32 start, const void *datap, u32 size);

/*---------------------------------------------------------------------------*
  Name:         SVC_IsMmemExpanded

  Description:  check if main memory is expanded

              �E���C����������8M�o�C�g�֊g������Ă��邩�ǂ����𒲂ׂ܂��B
              �EARM9���ł�0x023FFFF8�Ԓn��0x027FFFF8�Ԓn���L���b�V�������̐ݒ��
                ���Ă����K�v������܂��B

  Arguments:    None

  Returns:      TRUE if main memory is expanded

 *---------------------------------------------------------------------------*/
BOOL    SVC_IsMmemExpanded(void);

#endif // SDK_TS

#if defined(SDK_ARM7) || (defined(SDK_ARM9) && defined(SDK_TS))
/*---------------------------------------------------------------------------*
  Name:         SVC_Div

  Description:  quotient of division

              �Enumer/denom���Z�o���܂��B
              �E���W�X�^�̒l�́Ar0=numer/denom, r1=number%denom, 
                r3=|numer/denom|�ŕ��A���܂��B
              �E�R�[�h�T�C�Y��}���Ă��邽�߁A���܂荂���ł͂���܂���B

              �E�����F
                   number        ���q
                   denom         ����

  Arguments:    numer  : 
                denom  : 

  Returns:      quotient

 *---------------------------------------------------------------------------*/
s32     SVC_Div(s32 number, s32 denom);

/*---------------------------------------------------------------------------*
  Name:         SVC_DivRem

  Description:  remainder of division

              �Enumer%denom���Z�o���܂��B
              �E���W�X�^�̒l�́Ar0=number%denom, r1=number%denom, 
                r3=|numer/denom|�ŕ��A���܂��B
              �E�R�[�h�T�C�Y��}���Ă��邽�߁A���܂荂���ł͂���܂���B

              �E�����F
                   number        ���q
                   denom         ����

  Arguments:    numer  : 
                denom  : 

  Returns:      remainder

 *---------------------------------------------------------------------------*/
s32     SVC_DivRem(s32 number, s32 denom);

/*---------------------------------------------------------------------------*
  Name:         SVC_Sqrt

  Description:  square root

              �E���������Z�o���܂��B
              �E���x��ǂ����邽�߂Ɉ�����2�̔{���������V�t�g���ēn���A
                �߂�l���V�t�g���Č����킹���s�Ȃ��ĉ������B
              �E�R�[�h�T�C�Y��}���Ă��邽�߁A���܂荂���ł͂���܂���B

  Arguments:    src  : 

  Returns:      square root

 *---------------------------------------------------------------------------*/
u16     SVC_Sqrt(u32 src);

/*---------------------------------------------------------------------------*
  Name:         SVC_Halt

  Description:  halt

                �ECPU�R�A�̂ݒ�~�����܂��B
                �E�Y�����銄�荞�݂����iIE�ɃZ�b�g�j����Ă���
                  ���荞�ݗv���iIF�Z�b�g�j�ɂĕ��A���܂��B
                �ECPSR��IRQ�f�B�Z�[�u���t���O���Z�b�g����Ă���ꍇ
                  �iOS_DisableInterrupts�j�ɂ́A�z�[���g���畜�A���܂���
                  ���荞�݂͔������܂���B
                �EIME���N���A���ꂽ��ԂŃz�[���g�ɓ���܂��ƁiOS_DisableIrq�j
                  ���A�ł��Ȃ��Ȃ�܂��B

  Arguments:    None

  Returns:      None

 *---------------------------------------------------------------------------*/
void    SVC_Halt(void);

#endif // SDK_ARM7 || (SDK_ARM9 && SDK_TS)

#ifdef SDK_ARM7
/*---------------------------------------------------------------------------*
  Name:         SVC_Sleep

  Description:  sleep

              �E�����U���~���܂��B
              �ERTC�^�L�[�^�J�[�h�^�J�[�g���b�W�^�{�̃I�[�v���̂����ꂩ��
                ���荞�݂����iIE�ɃZ�b�g�j����Ă���ꍇ�A�Y�����銄�荞�ݗv������
                �̔����ɂ���ĕ��A���܂��B
              �E�����U����~���Ă��܂��̂ŕ��A�����IF�t���O�̓Z�b�g����܂��񂪁A
                CPU�ċN�����܂Œ[�q�֊��荞�ݗv���M�����������܂܂ɂȂ��Ă���ꍇ�ɂ�
                ���̎��_��IF�t���O���Z�b�g����܂��B
              �E���炩���ߗ��v���Z�b�T�Ƃ���POWCNT���W�X�^��0�N���A����
                �S�u���b�N���~�����A�T�E���h�A���v�△�����W���[������~�A
                ARM9�̓z�[���g��Ԃɂ��Ă���Ăяo���ĉ������B
              �EPOWCNT���W�X�^��LCD�C�l�[�u���t���O�́A���̊֐����Ăяo��
                100ms�ȏ�O��0�֗��Ƃ��ĉ������B����Ă��Ȃ��ꍇ��
                �{�̂��V���b�g�_�E�����Ă��܂��\��������܂��B

  Arguments:    None

  Returns:      None

 *---------------------------------------------------------------------------*/
void    SVC_Sleep(void);

/*---------------------------------------------------------------------------*
  Name:         SVC_SetSoundBias

  Description:  set sound bias

              �E�T�E���hBIAS��0���璆�Ԓl�i0x200�j�ֈڍs���܂��B

              �E�����F
                   stepLoops     �T�E���h�o�C�A�X�ύX�P�X�e�b�v�Ԃ̃��[�v���i�S�T�C�N���^���[�v�j�B
                                 �l���傫���قǃT�E���h�o�C�A�X���ɂ₩�ɕω������܂��B

  Arguments:    stepLoops : 

  Returns:      None

 *---------------------------------------------------------------------------*/
void    SVC_SetSoundBias(s32 stepLoops);

/*---------------------------------------------------------------------------*
  Name:         SVC_ResetSoundBias

  Description:  set sound bias

              �E�T�E���hBIAS�𒆊Ԓl�i0x200�j����0�ֈڍs���܂��B

              �E�����F
                   stepLoops     �T�E���h�o�C�A�X�ύX�P�X�e�b�v�Ԃ̃��[�v���i�S�T�C�N���^���[�v�j�B
                                 �l���傫���قǃT�E���h�o�C�A�X���ɂ₩�ɕω������܂��B

  Arguments:    stepLoops : 

  Returns:      None

 *---------------------------------------------------------------------------*/
void    SVC_ResetSoundBias(s32 stepLoops);

#ifdef SDK_TS
/*---------------------------------------------------------------------------*
  Name:         SVC_GetSinTable
                SVC_GetPitchTable
                SVC_GetVolumeTable

  Description:  get sound table data

              �E�T�E���h�֘A�̃e�[�u�����Q�Ƃ��Ēl��Ԃ��܂��B

              �E�����F
                   index         �C���f�b�N�X

  Arguments:    index : 

  Returns:      sound table data

 *---------------------------------------------------------------------------*/
s16     SVC_GetSinTable(int index);
u16     SVC_GetPitchTable(int index);
u8      SVC_GetVolumeTable(int index);

#endif // SDK_TS

#endif // SDK_ARM7


#ifdef __cplusplus
} /* extern "C" */
#endif

/* NITRO_OS_SYSTEMCALL_H_ */
#endif
