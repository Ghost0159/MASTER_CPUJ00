/*---------------------------------------------------------------------------*
  Project:  NitroSDK - MI - 
  File:     dma_red.h

  Copyright 2003-2005 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: dma_red.h,v $
  Revision 1.4  2005/03/01 01:57:00  yosizaki
  copyright �̔N���C��.

  Revision 1.3  2005/02/28 05:26:05  yosizaki
  do-indent.

  Revision 1.2  2004/02/05 07:09:02  yasu
  change SDK prefix iris -> nitro

  Revision 1.1  2003/12/18 07:20:07  yada
  red-sdk �̒�`���c������


  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#ifndef NITRO_MI_DMA_RED_H_
#define NITRO_MI_DMA_RED_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <nitro/misc.h>
#include <nitro/types.h>
#include <nitro/memorymap.h>
#include <nitro/os/systemCall.h>
#include <nitro/mi/dma.h>

//----------------------------------------------------------------
//      �J���Z�p�쐬�����C���^�t�F�[�X
//      �v�]�ɂ��c���Ă��܂��B
//      nitro-sdk/include/nitro/mi/dma.h �ɓ��ꂸ�ɂ����ɁB
//

//----------------------------------------------------------------------
//                      �c�l�` �Z�b�g
//----------------------------------------------------------------------
//#define SIMULATOR
#ifndef SIMULATOR
#define __MI_DmaSet(dmaNo, srcp, destp, dmaCntData)                       \
{                                                                    \
    vu32 *dmaCntp = &((vu32 *)REG_DMA0SAD_ADDR)[dmaNo * 3];                  \
    dmaCntp[0] = (vu32 )(srcp);                                      \
    dmaCntp[1] = (vu32 )(destp);                                     \
    dmaCntp[2] = (vu32 )(dmaCntData);                                \
    {u32 dummy = dmaCntp[2];}                                        \
    {u32 dummy = dmaCntp[2];}                                        \
}
#define __MI_DmaSetAsync(dmaNo, srcp, destp, dmaCntData)                  \
{                                                                    \
    vu32 *dmaCntp = &((vu32 *)REG_DMA0SAD_ADDR)[dmaNo * 3];                  \
    dmaCntp[0] = (vu32 )(srcp);                                      \
    dmaCntp[1] = (vu32 )(destp);                                     \
    dmaCntp[2] = (vu32 )(dmaCntData);                                \
}
#else
#define __MI_DmaSet(dmaNo, srcp, destp, dmaCntData)                       \
{                                                                    \
    int    i;                                                        \
    for (i=0; i<(dmaCntData & 0x1ffff); i++)                         \
        if ((dmaCntData) & MI_DMA_SRC_FIX) {                            \
            if ((dmaCntData) & MI_DMA_32BIT_BUS)                        \
                    ((vu32 *)(destp))[i] = ((vu32 *)(srcp))[0];      \
            else    ((vu16 *)(destp))[i] = ((vu16 *)(srcp))[0];      \
         } else {                                                    \
            if ((dmaCntData) & MI_DMA_32BIT_BUS)                        \
                ((vu32 *)(destp))[i] = ((vu32 *)(srcp))[i];          \
            else    ((vu16 *)(destp))[i] = ((vu16 *)(srcp))[i];      \
        }                                                            \
}
#define __MI_DmaSetAsync(dmaNo, srcp, destp, dmaCntData)                  \
        __MI_DmaSet(     dmaNo, srcp, destp, dmaCntData)
#endif

//�EDMA�R���g���[���Ƀp�����[�^���Z�b�g���܂��B
//�ESIMULATER���`�����CPU�ŃV�~�����[�g���܂��B
//  GDB�Ńf�o�b�O����ꍇ�ȂǂɗL���ł��B
//�E�Ō��DMA�N���҂��̂��߂�"LDR"���߂��}������܂��B
//
//�E�����F
//  dmaNo       DMA�ԍ�
//  srcp        �\�[�X�A�h���X
//  destp       �f�X�e�B�l�[�V�����A�h���X
//  dmaCntData  �p�����[�^�f�[�^
//
//��CPU����RAM��̃v���O�����ɂ�DMA���N�������
//  ���̎��̖��߂���Ɏ��s����܂��B
//  ����āADMA����ɓ]�����CPU�ɂĕύX���悤�Ƃ����ꍇ�A
//  �ǂݍ��݁^�����߂��̊Ԃ�DMA���N������Ă��܂��܂��̂ŁA
//  DMA�̓]����̃f�[�^���Ӑ}���Ă��Ȃ��l�ɂȂ�ꍇ������܂��B
//  ���̏ꍇ�ɂ�WaitDma()�𒼌�ɑ}�����āADMA���I��������
//  �ǂ������`�F�b�N����Ƒ����R�[�h�ւ̉e�����m���ɉ���ł��܂��B


//----------------------------------------------------------------------
//                      �c�l�` �N���A
//----------------------------------------------------------------------

#define __MI_DmaClear(dmaNo, data, destp, size, bit)             \
{                                                           \
    *(vu##bit *)HW_DMA_CLEAR_DATA_BUF = (vu##bit )(data);           \
    __MI_DmaSet(dmaNo, HW_DMA_CLEAR_DATA_BUF, destp, (                  \
        MI_DMA_ENABLE         | MI_DMA_TIMING_IMM |              \
        MI_DMA_SRC_FIX        | MI_DMA_DEST_INC    |              \
        MI_DMA_##bit##BIT_BUS | ((size)/(bit/8))));            \
}

#define __MI_DmaClearIf(dmaNo, data, destp, size, bit)           \
{                                                           \
    *(vu##bit *)DMA_CLEAR_DATA_BUF = (vu##bit )(data);       \
    __MI_DmaSet(dmaNo, DMA_CLEAR_DATA_BUF, destp, (                \
        MI_DMA_ENABLE         | MI_DMA_TIMING_IMM |              \
        MI_DMA_IF_ENABLE      |                                \
        MI_DMA_SRC_FIX        | MI_DMA_DEST_INC    |              \
        MI_DMA_##bit##BIT_BUS | ((size)/(bit/8))));            \
}

#define __MI_DmaClearArray(  dmaNo, data, destp, bit)            \
    __MI_DmaClear(       dmaNo, data, destp, sizeof(destp), bit)

#define MI_DmaClearArrayIf(dmaNo, data, destp, bit)            \
    __MI_DmaClearIf(     dmaNo, data, destp, sizeof(destp), bit)

//�EDMA��RAM�N���A���܂��B
//�E�N���A�f�[�^�̓X�^�b�N�ɒu����A������f�X�e�B�l�[�V�����փR�s�[���܂��B
//�EMI_DmaClearIf�^MI_DmaClearArrayIf�͏I�����Ɋ��荞�ݗv���𔭐����܂��B
//�EMI_DmaClearArray�^MI_DmaClearArrayIf�̓f�X�e�B�l�[�V�����z��S�̂��N���A���܂��B
//
//�E�����F
//  dmaNo       DMA�ԍ�
//  data        �N���A�f�[�^
//  destp       �f�X�e�B�l�[�V�����A�h���X
//  size        �N���A�o�C�g��
//  bit         �]���r�b�g���i16|32�j
//
//
//��CPU����RAM��̃v���O�����ɂ�DMA���N�������
//  ���̎��̖��߂���Ɏ��s����܂��B
//  ����āADMA����ɓ]�����CPU�ɂĕύX���悤�Ƃ����ꍇ�A
//  �ǂݍ��݁^�����߂��̊Ԃ�DMA���N������Ă��܂��܂��̂ŁA
//  DMA�̓]����̃f�[�^���Ӑ}���Ă��Ȃ��l�ɂȂ�ꍇ������܂��B
//  ���̏ꍇ�ɂ�WaitDma()�𒼌�ɑ}�����āADMA���I��������
//  �ǂ������`�F�b�N����Ƒ����R�[�h�ւ̉e�����m���ɉ���ł��܂��B

//----------------------------------------------------------------------
//                      �c�l�` �R�s�[
//----------------------------------------------------------------------

#define __MI_DmaCopy(dmaNo, srcp, destp, size, bit)              \
                                                            \
    __MI_DmaSet(dmaNo, srcp, destp,  (                           \
        MI_DMA_ENABLE         | MI_DMA_TIMING_IMM |              \
        MI_DMA_SRC_INC        | MI_DMA_DEST_INC    |              \
        MI_DMA_##bit##BIT_BUS | ((size)/((bit)/8))))

#define __MI_DmaCopyIf(dmaNo, srcp, destp, size, bit)            \
                                                            \
    __MI_DmaSet(dmaNo, srcp, destp,  (                           \
        MI_DMA_ENABLE         | MI_DMA_TIMING_IMM |              \
        MI_DMA_IF_ENABLE      |                                \
        MI_DMA_SRC_INC        | MI_DMA_DEST_INC    |              \
        MI_DMA_##bit##BIT_BUS | ((size)/(bit/8))))

#define __MI_DmaCopyArray(  dmaNo, srcp, destp, bit)             \
    __MI_DmaCopy(       dmaNo, srcp, destp, sizeof(srcp), bit)

#define __MI_DmaCopyArrayIf(dmaNo, srcp, destp, bit)             \
    __MI_DmaCopyIf(     dmaNo, srcp, destp, sizeof(srcp), bit)

//�EDMA�ŃR�s�[���܂��B
//�EMI_DmaCopyIf�^MI_DmaCopyArrayIf�͏I�����Ɋ��荞�ݗv���𔭐����܂��B
//�EMI_DmaCopyArray�^MI_DmaCopyArrayIf�̓\�[�X�z��S�̂��R�s�[���܂��B
//
//�E�����F
//  dmaNo       DMA�ԍ�
//  srcp        �\�[�X�A�h���X
//  destp       �f�X�e�B�l�[�V�����A�h���X
//  size        �]���o�C�g��
//  bit         �]���r�b�g���i16|32�j
//
//
//��CPU����RAM��̃v���O�����ɂ�DMA���N�������
//  ���̎��̖��߂���Ɏ��s����܂��B
//  ����āADMA����ɓ]�����CPU�ɂĕύX���悤�Ƃ����ꍇ�A
//  �ǂݍ��݁^�����߂��̊Ԃ�DMA���N������Ă��܂��܂��̂ŁA
//  DMA�̓]����̃f�[�^���Ӑ}���Ă��Ȃ��l�ɂȂ�ꍇ������܂��B
//  ���̏ꍇ�ɂ�WaitDma()�𒼌�ɑ}�����āADMA���I��������
//  �ǂ������`�F�b�N����Ƒ����R�[�h�ւ̉e�����m���ɉ���ł��܂��B

//----------------------------------------------------------------------
//                  �g�u�����N�c�l�` �R�s�[
//----------------------------------------------------------------------

#define __MI_H_DmaCopy(dmaNo, srcp, destp, size, bit)            \
                                                            \
    __MI_DmaSet(dmaNo, srcp, destp,  (                           \
        MI_DMA_ENABLE         | MI_DMA_TIMING_H_BLANK |          \
        MI_DMA_SRC_INC        | MI_DMA_DEST_RELOAD     |          \
        MI_DMA_CONTINUOUS_ON  |                                \
        MI_DMA_##bit##BIT_BUS | ((size)/((bit)/8))))

#define __MI_H_DmaCopyIf(dmaNo, srcp, destp, size, bit)          \
                                                            \
    __MI_DmaSet(dmaNo, srcp, destp,  (                           \
        MI_DMA_ENABLE         | MI_DMA_TIMING_H_BLANK |          \
        MI_DMA_IF_ENABLE      |                                \
        MI_DMA_SRC_INC        | MI_DMA_DEST_RELOAD     |          \
        MI_DMA_CONTINUOUS_ON  |                                \
        MI_DMA_##bit##BIT_BUS | ((size)/(bit/8))))

#define __MI_H_DmaCopyArray(  dmaNo, srcp, destp, bit)           \
    __MI_H_DmaCopy(       dmaNo, srcp, destp, sizeof(srcp), bit)

#define __MI_H_DmaCopyArrayIf(dmaNo, srcp, destp, bit)           \
    __MI_H_DmaCopyIf(     dmaNo, srcp, destp, sizeof(srcp), bit)

//�EH�u�����N�ɓ�������DMA�ŃR�s�[���܂��B
//�EMI_H_DmaCopyIf�^MI_H_DmaCopyArrayIf�͏I�����Ɋ��荞�ݗv���𔭐����܂��B
//�EMI_H_DmaCopyArray�^MI_H_DmaCopyArrayIf�̓\�[�X�z��S�̂��R�s�[���܂��B
//
//�E�����F
//  dmaNo       DMA�ԍ�
//  srcp        �\�[�X�A�h���X
//  destp       �f�X�e�B�l�[�V�����A�h���X
//  size        �]���o�C�g��
//  bit         �]���r�b�g���i16|32�j

//----------------------------------------------------------------------
//                  �u�u�����N�c�l�` �R�s�[
//----------------------------------------------------------------------

#define __MI_V_DmaCopy(dmaNo, srcp, destp, size, bit)            \
                                                            \
    __MI_DmaSet(dmaNo, srcp, destp,  (                           \
        MI_DMA_ENABLE         | MI_DMA_TIMING_V_BLANK |          \
        MI_DMA_SRC_INC        | MI_DMA_DEST_INC        |          \
        MI_DMA_##bit##BIT_BUS | ((size)/(bit/8))))

#define __MI_V_DmaCopyIf(dmaNo, srcp, destp, size, bit)          \
                                                            \
    __MI_DmaSet(dmaNo, srcp, destp,  (                           \
        MI_DMA_ENABLE         | MI_DMA_TIMING_V_BLANK |          \
        MI_DMA_IF_ENABLE      |                                \
        MI_DMA_SRC_INC        | MI_DMA_DEST_INC        |          \
        MI_DMA_##bit##BIT_BUS | ((size)/(bit/8))))

#define __MI_V_DmaCopyArray(  dmaNo, srcp, destp, bit)           \
    __MI_V_DmaCopy(       dmaNo, srcp, destp, sizeof(srcp), bit)

#define __MI_V_DmaCopyArrayIf(dmaNo, srcp, destp, bit)           \
    __MI_V_DmaCopyIf(     dmaNo, srcp, destp, sizeof(srcp), bit)

//�EV�u�����N�ɓ�������DMA�ŃR�s�[���܂��B
//�EMI_V_DmaCopyIf�^MI_V_DmaCopyArrayIf�͏I�����Ɋ��荞�ݗv���𔭐����܂��B
//�EMI_V_DmaCopyArray�^MI_V_DmaCopyArrayIf�̓\�[�X�z��S�̂��R�s�[���܂��B
//
//�E�����F
//  dmaNo       DMA�ԍ�
//  srcp        �\�[�X�A�h���X
//  destp       �f�X�e�B�l�[�V�����A�h���X
//  size        �]���o�C�g��
//  bit         �]���r�b�g���i16|32�j

//----------------------------------------------------------------------
//                  ���C���������\���c�l�`
//----------------------------------------------------------------------

#define __MI_DmaDispMainmem(dmaNo, srcp)                         \
                                                            \
    __MI_DmaSet(dmaNo, srcp, REG_DISP_MMEM_FIFO_ADDR,  (              \
        MI_DMA_ENABLE         | MI_DMA_TIMING_DISP_MMEM |        \
        MI_DMA_SRC_INC        | MI_DMA_DEST_FIX          |        \
		MI_DMA_CONTINUOUS_ON  |								\
        MI_DMA_32BIT_BUS      | (4)))

//�E���C����������̃C���[�W��\������DMA�]�����s���܂��B
//
//�E�����F
//  dmaNo       DMA�ԍ�
//  srcp        �\�[�X�A�h���X

//----------------------------------------------------------------------
//                  �W�I���g���e�h�e�n�|�c�l�`
//----------------------------------------------------------------------

#define __MI_GX_Dma(dmaNo, srcp, length)                         \
                                                            \
    __MI_DmaSetAsync(dmaNo, srcp, REG_GXFIFO_ADDR,  (                 \
        MI_DMA_ENABLE         | MI_DMA_TIMING_GXFIFO  |          \
        MI_DMA_SRC_INC        | MI_DMA_DEST_FIX        |          \
        MI_DMA_32BIT_BUS      | (length)))

#define __MI_GX_DmaIf(dmaNo, srcp, length)                       \
                                                            \
    __MI_DmaSetAsync(dmaNo, srcp, REG_GXFIFO_ADDR  (                 \
        MI_DMA_ENABLE         | MI_DMA_TIMING_GXFIFO  |          \
        MI_DMA_IF_ENABLE      |                                \
        MI_DMA_SRC_INC        | MI_DMA_DEST_FIX        |          \
        MI_DMA_32BIT_BUS      | (length)))

#define __MI_GX_DmaFast(dmaNo, srcp, length)                     \
                                                            \
    __MI_DmaSetAsync(dmaNo, srcp, REG_GXFIFO_ADDR,  (                 \
        MI_DMA_ENABLE         | MI_DMA_TIMING_IMM     |          \
        MI_DMA_SRC_INC        | MI_DMA_DEST_FIX        |          \
        MI_DMA_32BIT_BUS      | (length)))

#define __MI_GX_DmaFastIf(dmaNo, srcp, length)                   \
                                                            \
    __MI_DmaSetAsync(dmaNo, srcp, REG_GXFIFO_ADDR,  (                 \
        DMA_ENABLE         | DMA_TIMING_IMM     |          \
        DMA_IF_ENABLE      |                                \
        DMA_SRC_INC        | DMA_DEST_FIX        |          \
        DMA_32BIT_BUS      | (length)))

#define __MI_GX_DmaArray(  dmaNo, srcp, destp, bit)              \
    __MI_GX_Dma(  dmaNo, srcp, destp, sizeof(srcp), bit)

#define __MI_GX_DmaArrayIf(dmaNo, srcp, destp, bit)              \
    __MI_GX_DmaIf(dmaNo, srcp, destp, sizeof(srcp), bit)

#define __MI_GX_DmaArrayFast(  dmaNo, srcp, destp, bit)          \
    __MI_GX_DmaFast(  dmaNo, srcp, destp, sizeof(srcp), bit)

#define __MI_GX_DmaArrayFastIf(dmaNo, srcp, destp, bit)          \
    __MI_GX_DmaFastIf(dmaNo, srcp, destp, sizeof(srcp), bit)

//�E�W�I���g��FIFO����̗v���ɂ����DMA�ŃR�s�[���܂��B
//�EMI_GX_DmaIf�^MI_GX_DmaArrayIf�^MI_GX_DmaFastIf�^MI_GX_DmaArrayFastIf
//  �͏I�����Ɋ��荞�ݗv���𔭐����܂��B
//�EMI_GX_DmaArray�^MI_GX_DmaArrayIf�^MI_GX_DmaArrayFast�^MI_GX_DmaArrayFastIf
//  �̓\�[�X�z��S�̂��R�s�[���܂��B
//
//�E�����F
//  dmaNo       DMA�ԍ�
//  srcp        �\�[�X�A�h���X
//  size        �]���o�C�g��


//----------------------------------------------------------------------
//                      �c�l�` �I���҂�
//----------------------------------------------------------------------

#define __MI_WaitDma(dmaNo)                                      \
{                                                           \
    vu32 *(dmaCntp) = &((vu32 *)REG_DMA0SAD_ADDR)[dmaNo * 3];       \
    while (dmaCntp[2] & MI_DMA_ENABLE) ;                       \
}

//�EDMA�̏I����҂��܂��B
//
//�E�����F
//  dmaNo       DMA�ԍ�


//----------------------------------------------------------------------
//                      �c�l�` �X�g�b�v
//----------------------------------------------------------------------

#define __MI_StopDma(dmaNo)                                      \
{                                                           \
    vu16* dmaCntp = &((vu16 *)REG_DMA0SAD_ADDR)[dmaNo * 6];       \
    dmaCntp[5] &= ~((MI_DMA_TIMING_MASK | MI_DMA_CONTINUOUS_ON)  \
                   >> 16);                                  \
    dmaCntp[5] &= ~( MI_DMA_ENABLE   >> 16);                   \
    {u32 dummy = dmaCntp[5];}                               \
    {u32 dummy = dmaCntp[5];}                               \
}

//�EDMA���~���܂��B
//�E�A���A��������DMA�̎����N�����|�������ꍇ�͈�x����DMA�����s����܂��B
//
//�E�����F
//  dmaNo       DMA�ԍ�


//----------------------------------------------------------------------
//                      �b�o�t �N���A
//----------------------------------------------------------------------

#define __MI_CpuClear(data, destp, size, bit)    UTL_CpuClear##bit(data, (void *)(destp), size)

#define __MI_CpuClearArray(data, destp, bit)                     \
    __MI_CpuClear(     data, destp, sizeof(destp), bit)

//�ECPU��RAM�N���A����V�X�e���R�[�����Ăяo���܂��B
//�E�N���A�f�[�^�̓X�^�b�N�ɒu����A������f�X�e�B�l�[�V�����փR�s�[���܂��B
//�ECpuClearArray�̓f�X�e�B�l�[�V�����z��S�̂��N���A���܂��B
//
//�E�����F
//  data        �N���A�f�[�^
//  destp       �f�X�e�B�l�[�V�����A�h���X
//  size        �N���A�o�C�g��
//  bit         �]���r�b�g���i16|32�j

//----------------------------------------------------------------------
//                      �b�o�t �R�s�[
//----------------------------------------------------------------------

#define __MI_CpuCopy(srcp, destp, size, bit)    UTL_CpuCopy##bit((void *)(srcp), (void *)(destp), size)

#define __MI_CpuCopyArray(srcp, destp, bit)                      \
    __MI_CpuCopy(     srcp, destp, sizeof(srcp), bit)

//�ECPU�ŃR�s�[����V�X�e���R�[�����Ăяo���܂��B
//�ECpuCopyArray�̓\�[�X�z��S�̂��R�s�[���܂��B
//
//�E�����F
//  srcp        �\�[�X�A�h���X
//  destp       �f�X�e�B�l�[�V�����A�h���X
//  size        �]���o�C�g��
//  bit         �]���r�b�g���i16|32�j

//----------------------------------------------------------------------
//                  �b�o�t �����N���A(32Byte�P��)
//----------------------------------------------------------------------

#define __MI_CpuClearFast(data, destp, size)  UTL_CpuClearFast(data, (void *)(destp), size)

#define MI_CpuClearArrayFast(data, destp)                      \
    __MI_CpuClearFast(     data, destp, sizeof(destp))

//�ECPU�ō�����RAM�N���A����V�X�e���R�[�����Ăяo���܂��B
//�E�N���A�f�[�^�̓X�^�b�N�ɒu����A������f�X�e�B�l�[�V�����փR�s�[���܂��B
//�ECpuClearArrayFast�̓f�X�e�B�l�[�V�����z��S�̂��N���A���܂��B
//
//�E�����F
//  data        �N���A�f�[�^
//  destp       �f�X�e�B�l�[�V�����A�h���X
//  size        �N���A�o�C�g��

//----------------------------------------------------------------------
//                  �b�o�t �����R�s�[(32Byte�P��)
//----------------------------------------------------------------------

#define __MI_CpuCopyFast(srcp, destp, size)   UTL_CpuCopyFast((void *)(srcp), (void *)(destp), size)


#define MI_CpuCopyArrayFast(srcp, destp)                       \
    __MI_CpuCopyFast(     srcp, destp, sizeof(srcp))

//�ECPU�ō����ɃR�s�[����V�X�e���R�[�����Ăяo���܂��B
//�ECpuCopyArrayFast�̓\�[�X�z��S�̂��R�s�[���܂��B
//
//�E�����F
//  srcp        �\�[�X�A�h���X
//  destp       �f�X�e�B�l�[�V�����A�h���X
//  size        �]���o�C�g��





#ifdef __cplusplus
} /* extern "C" */
#endif

/* NITRO_MI_DMA_RED_H_ */
#endif
