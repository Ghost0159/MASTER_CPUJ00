/*---------------------------------------------------------------------------*
  Project:  NitroSDK - HW - include
  File:     armArch.h

  Copyright 2003-2005 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: armArch.h,v $
  Revision 1.12  2005/03/01 01:57:00  yosizaki
  copyright �̔N���C��.

  Revision 1.11  2005/02/28 05:26:01  yosizaki
  do-indent.

  Revision 1.10  2004/05/03 04:50:46  yada
  add HW_CPU_CLOCK*

  Revision 1.9  2004/04/07 02:03:17  yada
  fix header comment

  Revision 1.8  2004/03/16 11:53:06  yasu
  move HW_READ32/WRITE32 -> MI_ReadWord/WriteWord

  Revision 1.7  2004/02/25 11:28:59  yada
  HW_SYSTEM_CLOCK �ǉ�

  Revision 1.6  2004/02/14 06:32:31  yasu
  add HW_READ43/WRITE32 macros

  Revision 1.5  2004/02/05 07:09:03  yasu
  change SDK prefix iris -> nitro

  Revision 1.4  2003/12/18 01:52:47  yada
  �f�[�^�L���b�V���A���߃L���b�V���C�l�[�u���t���O�̃V�t�g�ʒǉ�

  Revision 1.3  2003/12/17 11:38:48  yada
  �L���b�V����`�ǉ�

  Revision 1.2  2003/12/12 01:29:00  yada
  HW_PSR_ARM_STATE �ǉ�

  Revision 1.1  2003/12/12 01:24:46  yada
  hw/systemControl.h �� hw/armArch.h �Ɩ��̕ύX

  Revision 1.2  2003/11/28 01:55:16  yada
  REDSDK��03-11-27���f

  Revision 1.1  2003/11/04 09:11:24  yada
  ����(�����������܂��Ƃ�Ă��Ȃ���������܂���)


  $NoKeywords: $

 *---------------------------------------------------------------------------*/

#ifndef NITRO_HW_SYSTEMCONTROL_H_
#define NITRO_HW_SYSTEMCONTROL_H_

#ifdef __cplusplus
extern "C" {
#endif

#define HW_ICACHE_SIZE              0x2000      //   ���߃L���b�V��
#define HW_DCACHE_SIZE              0x1000      // �f�[�^�L���b�V��
#define HW_CACHE_LINE_SIZE          32

#define HW_SYSTEM_CLOCK             33514000    // ���m�ɂ�33513982?

#define HW_CPU_CLOCK_ARM7           33513982
#define HW_CPU_CLOCK_ARM9           67027964

#ifdef SDK_ARM7
#define HW_CPU_CLOCK                HW_CPU_CLOCK_ARM7
#else
#define HW_CPU_CLOCK                HW_CPU_CLOCK_ARM9
#endif

//**********************************************************************
//              �h�q�h�r���W�X�^�E�R���g���[���r�b�g
//**********************************************************************

//----------------------------------------------------------------------
//                  �v���O�����X�e�[�^�X���W�X�^
//----------------------------------------------------------------------

#define HW_PSR_CPU_MODE_MASK       0x1f // �b�o�t���[�h

#define HW_PSR_USER_MODE           0x10 // ���[�U
#define HW_PSR_FIQ_MODE            0x11 // �e�h�p
#define HW_PSR_IRQ_MODE            0x12 // �h�q�p
#define HW_PSR_SVC_MODE            0x13 // �X�[�p�o�C�U
#define HW_PSR_ABORT_MODE          0x17 // �A�{�[�g�i�v���t�F�b�`�^�f�[�^�j
#define HW_PSR_UNDEF_MODE          0x1b // ����`����
#define HW_PSR_SYS_MODE            0x1f // �V�X�e��

#define HW_PSR_ARM_STATE           0x0 // �`�q�l�X�e�[�g
#define HW_PSR_THUMB_STATE         0x20 // �s�g�t�l�a�X�e�[�g

#define HW_PSR_FIQ_DISABLE         0x40 // �e�h�p�s����
#define HW_PSR_IRQ_DISABLE         0x80 // �h�q�p�s����
#define HW_PSR_IRQ_FIQ_DISABLE     0xc0 // �h�q�p���e�h�p�s����

#define HW_PSR_Q_FLAG              0x08000000   // �X�e�B�b�L�[�I�[�o�[�t���[
#define HW_PSR_V_FLAG              0x10000000   // �I�[�o�[�t���[
#define HW_PSR_C_FLAG              0x20000000   // �L�����[�^�{���[�^�g��
#define HW_PSR_Z_FLAG              0x40000000   // �[��
#define HW_PSR_N_FLAG              0x80000000   // ���^����


//----------------------------------------------------------------------
//                  �V�X�e���R���g���[���R�v���Z�b�T
//----------------------------------------------------------------------

// ���W�X�^�P�i�}�X�^�R���g���[���j

#define HW_C1_SB1_BITSET           0x00000078   // ���W�X�^�P�p�P�Œ�r�b�g��

#define HW_C1_ITCM_LOAD_MODE       0x00080000   //   ���߂s�b�l ���[�h���[�h
#define HW_C1_DTCM_LOAD_MODE       0x00020000   // �f�[�^�s�b�l ���[�h���[�h
#define HW_C1_ITCM_ENABLE          0x00040000   //   ���߂s�b�l �C�l�[�u��
#define HW_C1_DTCM_ENABLE          0x00010000   // �f�[�^�s�b�l �C�l�[�u��
#define HW_C1_LD_INTERWORK_DISABLE 0x00008000   // ���[�h���߂ɂ��C���^�[���[�L���O �f�B�Z�[�u��
#define HW_C1_CACHE_ROUND_ROBIN    0x00004000   // �L���b�V���u���A���S���Y�� ���E���h���r���i�ň����̃q�b�g��������j
#define HW_C1_CACHE_PSEUDO_RANDOM  0x00000000   //                            �[�������_��
#define HW_C1_EXCEPT_VEC_UPPER     0x00002000   // ��O�x�N�^ ��ʃA�h���X�i������ɐݒ肵�ĉ������j
#define HW_C1_EXCEPT_VEC_LOWER     0x00000000   //            ���ʃA�h���X
#define HW_C1_ICACHE_ENABLE        0x00001000   //   ���߃L���b�V�� �C�l�[�u��
#define HW_C1_DCACHE_ENABLE        0x00000004   // �f�[�^�L���b�V�� �C�l�[�u��
#define HW_C1_LITTLE_ENDIAN        0x00000000   // ���g���G���f�B�A��
#define HW_C1_BIG_ENDIAN           0x00000080   // �r�b�O�G���f�B�A��
#define HW_C1_PROTECT_UNIT_ENABLE  0x00000001   // �v���e�N�V�������j�b�g �C�l�[�u��

#define HW_C1_ICACHE_ENABLE_SHIFT 12
#define HW_C1_DCACHE_ENABLE_SHIFT 2


// ���W�X�^�Q�i�v���e�N�V�������[�W�����E�L���b�V���ݒ�j

#define HW_C2_PR0_SFT              0   // �v���e�N�V�������[�W�����O
#define HW_C2_PR1_SFT              1   //                 �P
#define HW_C2_PR2_SFT              2   //                 �Q
#define HW_C2_PR3_SFT              3   //                 �R
#define HW_C2_PR4_SFT              4   //                 �S
#define HW_C2_PR5_SFT              5   //                 �T
#define HW_C2_PR6_SFT              6   //                 �U
#define HW_C2_PR7_SFT              7   //                 �V


// ���W�X�^�R�i�v���e�N�V�������[�W�����E���C�g�o�b�t�@�ݒ�j

#define HW_C3_PR0_SFT              0   // �v���e�N�V�������[�W�����O
#define HW_C3_PR1_SFT              1   //                 �P
#define HW_C3_PR2_SFT              2   //                 �Q
#define HW_C3_PR3_SFT              3   //                 �R
#define HW_C3_PR4_SFT              4   //                 �S
#define HW_C3_PR5_SFT              5   //                 �T
#define HW_C3_PR6_SFT              6   //                 �U
#define HW_C3_PR7_SFT              7   //                 �V


// ���W�X�^�T�i�v���e�N�V�������[�W�����E�A�N�Z�X���j

#define HW_C5_PERMIT_MASK          0xf // �v���e�N�V�������[�W�����A�N�Z�X���}�X�N

#define HW_C5_PERMIT_NA            0   //                  �A�N�Z�X�s����
#define HW_C5_PERMIT_RW            1   //                  ���[�h���C�g����
#define HW_C5_PERMIT_RO            5   //                  ���[�h�I�����[����

#define HW_C5_PR0_SFT              0   // �v���e�N�V�������[�W�����O
#define HW_C5_PR1_SFT              4   //                 �P
#define HW_C5_PR2_SFT              8   //                 �Q
#define HW_C5_PR3_SFT              12  //                 �R
#define HW_C5_PR4_SFT              16  //                 �S
#define HW_C5_PR5_SFT              20  //                 �T
#define HW_C5_PR6_SFT              24  //                 �U
#define HW_C5_PR7_SFT              28  //                 �V


// ���W�X�^�U�i�v���e�N�V�������[�W�����E�x�[�X�A�h���X�^�T�C�Y�j

#define HW_C6_PR_SIZE_MASK         0x0000003e   // �v���e�N�V�������[�W���� �T�C�Y
#define HW_C6_PR_BASE_MASK         0xfffff000   //                  �x�[�X�A�h���X

#define HW_C6_PR_SIZE_SHIFT        1
#define HW_C6_PR_BASE_SHIFT        12

#define HW_C6_PR_ENABLE            1   // �v���e�N�V�������[�W���� �C�l�[�u��
#define HW_C6_PR_DISABLE           0   //                  �f�B�Z�[�u��

#define HW_C6_PR_4KB               0x16 // ���[�W�����T�C�Y �S�j�a������
#define HW_C6_PR_8KB               0x18 //                  �W�j�a������
#define HW_C6_PR_16KB              0x1a //                �P�U�j�a������
#define HW_C6_PR_32KB              0x1c //                �R�Q�j�a������
#define HW_C6_PR_64KB              0x1e //                �U�S�j�a������
#define HW_C6_PR_128KB             0x20 //              �P�Q�W�j�a������
#define HW_C6_PR_256KB             0x22 //              �Q�T�U�j�a������
#define HW_C6_PR_512KB             0x24 //              �T�P�Q�j�a������
#define HW_C6_PR_1MB               0x26 //                  �P�l�a������
#define HW_C6_PR_2MB               0x28 //                  �Q�l�a������
#define HW_C6_PR_4MB               0x2a //                  �S�l�a������
#define HW_C6_PR_8MB               0x2c //                  �W�l�a������
#define HW_C6_PR_16MB              0x2e //                �P�U�l�a������
#define HW_C6_PR_32MB              0x30 //                �R�Q�l�a������
#define HW_C6_PR_64MB              0x32 //                �U�S�l�a������
#define HW_C6_PR_128MB             0x34 //              �P�Q�W�l�a������
#define HW_C6_PR_256MB             0x36 //              �Q�T�U�l�a������
#define HW_C6_PR_512MB             0x38 //              �T�P�Q�l�a������
#define HW_C6_PR_1GB               0x3a //                  �P�f�a������
#define HW_C6_PR_2GB               0x3c //                  �Q�f�a������
#define HW_C6_PR_4GB               0x3e //                  �S�f�a������


// ���W�X�^�V.�P�R�i���߃L���b�V���E�v���t�F�b�`�j

#define HW_C7_ICACHE_PREFCHP_MASK  0xffffffe0   //   ���߃L���b�V�� �v���t�F�b�`�A�h���X


// ���W�X�^�V.�P�O�A�V.�P�S�i�L���b�V���C���f�b�N�X����j

#define HW_C7_ICACHE_INDEX_MASK    0x00000fe0   //   ���߃L���b�V�� �C���f�b�N�X
#define HW_C7_DCACHE_INDEX_MASK    0x000003e0   // �f�[�^�L���b�V�� �C���f�b�N�X
#define HW_C7_CACHE_SET_NO_MASK    0xc0000000   //       �L���b�V�� �Z�b�g�m��

#define HW_C7_CACHE_INDEX_SHIFT    5
#define HW_C7_CACHE_SET_NO_SHIFT   30


// ���W�X�^�X.�O�i�L���b�V�����b�N�_�E���j

#define HW_C9_LOCKDOWN_SET_NO_MASK 0x00000003   // �L���b�V�����b�N�_�E�� �Z�b�g�m��

#define HW_C9_LOCKDOWN_SET_NO_SHIFT 0

#define HW_C9_LOCKDOWN_LOAD_MODE   0x80000000   // �L���b�V�����b�N�_�E�� ���[�h���[�h


// ���W�X�^�X.�P�i�s�b�l�x�[�X�A�h���X�^�T�C�Y�j

#define HW_C9_TCMR_SIZE_MASK       0x0000003e   // �s�b�l���[�W���� �T�C�Y
#define HW_C9_TCMR_BASE_MASK       0xfffff000   //                  �x�[�X�A�h���X

#define HW_C9_TCMR_SIZE_SHIFT      1
#define HW_C9_TCMR_BASE_SHIFT      12

#define HW_C9_TCMR_4KB             0x06 // ���[�W�����T�C�Y �S�j�a������
#define HW_C9_TCMR_8KB             0x08 //                  �W�j�a������
#define HW_C9_TCMR_16KB            0x0a //                �P�U�j�a������
#define HW_C9_TCMR_32KB            0x0c //                �R�Q�j�a������
#define HW_C9_TCMR_64KB            0x0e //                �U�S�j�a������
#define HW_C9_TCMR_128KB           0x10 //              �P�Q�W�j�a������
#define HW_C9_TCMR_256KB           0x12 //              �Q�T�U�j�a������
#define HW_C9_TCMR_512KB           0x14 //              �T�P�Q�j�a������
#define HW_C9_TCMR_1MB             0x16 //                  �P�l�a������
#define HW_C9_TCMR_2MB             0x18 //                  �Q�l�a������
#define HW_C9_TCMR_4MB             0x1a //                  �S�l�a������
#define HW_C9_TCMR_8MB             0x1c //                  �W�l�a������
#define HW_C9_TCMR_16MB            0x1e //                �P�U�l�a������
#define HW_C9_TCMR_32MB            0x20 //                �R�Q�l�a������
#define HW_C9_TCMR_64MB            0x22 //                �U�S�l�a������
#define HW_C9_TCMR_128MB           0x24 //              �P�Q�W�l�a������
#define HW_C9_TCMR_256MB           0x26 //              �Q�T�U�l�a������
#define HW_C9_TCMR_512MB           0x28 //              �T�P�Q�l�a������
#define HW_C9_TCMR_1GB             0x2a //                  �P�f�a������
#define HW_C9_TCMR_2GB             0x2c //                  �Q�f�a������
#define HW_C9_TCMR_4GB             0x2e //                  �S�f�a������

#ifdef __cplusplus
} /* extern "C" */
#endif

/* NITRO_HW_SYSTEMCONTROL_H_ */
#endif
