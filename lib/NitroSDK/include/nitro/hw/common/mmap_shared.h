/*---------------------------------------------------------------------------*
  Project:  NitroSDK - HW - include
  File:     mmap_shared.h

  Copyright 2003-2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: mmap_shared.h,v $
  Revision 1.43  2006/08/10 00:05:10  okubata_ryoma
  �J�[�g���b�W�̊����}���Ɋւ���s��C��

  Revision 1.42  2006/07/05 09:14:54  yosizaki
  delete definitions of OSSystemWork. (moved to os/systemWork.h)

  Revision 1.41  2006/01/18 02:11:30  kitase_hirotake
  do-indent

  Revision 1.40  2005/09/08 12:28:09  yasu
  SDK_FROM_TOOL �̔��蕔���� SDK_WIN32 �ł̔�����ǉ�

  Revision 1.39  2005/07/27 07:45:30  seiki_masashi
  OSSystemWork �� wm_keyshare_control �� wm_rssi_pool �ɕύX

  Revision 1.38  2005/06/02 10:28:11  terui
  OSSystemWork�\���̓���file_control�y��is_reserved�����o�𓝍�����rom_header�����o�ɕϖ�

  Revision 1.37  2005/05/20 03:10:53  yosizaki
  add HW_CARD_ROM_HEADER, HW_DOWNLOAD_PARAMETER.

  Revision 1.36  2005/02/28 05:26:01  yosizaki
  do-indent.

  Revision 1.35  2004/12/02 00:26:02  yada
  add autoload sync

  Revision 1.34  2004/11/22 01:14:51  yada
  add HW_ROM_BASE_OFFSET_BUF

  Revision 1.33  2004/09/01 04:19:11  yada
  add HW_RESET_PARAMETER_BUF

  Revision 1.32  2004/08/17 04:09:56  yada
  add some definition to detect debugger correctly

  Revision 1.31  2004/08/16 08:16:16  yada
  HW_CHECK_DEBUGGER_BUF definition is moved to mmap_shared.h header

  Revision 1.30  2004/08/16 08:08:06  yada
  add HW_BOOT_CHECK_INFO_BUF and HW_CTRDG_MODULE_INFO_BUF

  Revision 1.29  2004/08/10 12:12:01  yada
  place VBlank count buffer in shared area

  Revision 1.28  2004/08/07 00:58:46  terui
  wm_control���폜�Awm_callback_control wm_keyshare_control��ǉ�

  Revision 1.27  2004/08/03 01:36:27  terui
  Add wm_control

  Revision 1.26  2004/07/27 05:59:11  yada
  fix OSSystemWork structure.
  lowest 0x80 (increased in recent release) byte was forgotten.

  Revision 1.25  2004/07/22 11:29:23  yada
  shared area is expanded 0x80 byte
  make HW_WM_BOOT_BUF

  Revision 1.24  2004/06/15 02:10:00  yada
  only add HW_DMA_CLEAR_DATA_BUF_END

  Revision 1.23  2004/06/08 10:41:55  yada
  only fix comment

  Revision 1.22  2004/06/01 04:24:24  terui
  Add mic_sampling_data.

  Revision 1.21  2004/05/28 09:40:30  terui
  Add mic_last_address

  Revision 1.20  2004/05/24 09:54:31  yasu
  move RED_RESERVED_END to (HW_MAIN_MEM + 0x007ffa00)

  Revision 1.19  2004/05/21 12:11:58  yasu
  Reduce the SHARED ARENA because of allocating HW_RED_RESERVED

  Revision 1.18  2004/05/21 11:07:26  yada
  add HW_RED_RESERVED area

  Revision 1.17  2004/05/19 05:19:17  yada
  add NVRAM user info area

  Revision 1.16  2004/05/14 11:58:24  takano_makoto
  Add #ifndef SDK_FROM_TOOL for VC or BCB.

  Revision 1.15  2004/05/11 12:08:13  yada
  some define names was changed,
  HW_BIOS_EXCP_STACK_MAIN, HW_EXCP_VECTOR_MAIN,
  HW_IS_RESERVED was deleted.

  Revision 1.14  2004/05/11 01:14:33  yasu
  Allocate PXI related flags on shared area

  Revision 1.13  2004/05/10 08:20:19  yada
  ram checker deleted, because put checker for main and sub.

  Revision 1.12  2004/05/10 08:15:46  yada
  HW_COMPONENT_PARAM �̈ʒu�ς�

  Revision 1.11  2004/05/06 00:38:14  yada
  add HW_IS_RESERVED

  Revision 1.10  2004/05/06 00:17:13  yada
  add HW_REAL_TIME_CLOCK_BUF

  Revision 1.9  2004/05/04 04:11:11  terui
  Change position of real time clock infromation area.

  Revision 1.8  2004/05/04 02:08:50  terui
  Add real time clock information area.

  Revision 1.7  2004/04/30 07:37:44  yada
  HW_LOCKIDFLAG_MAIN/SUB -> HW_LOCK_ID_FLAG_MAIN/SUB

  Revision 1.6  2004/04/29 09:34:02  yada
  put HW_LOCKID_FLAG_MAIN/SUB

  Revision 1.5  2004/04/14 05:05:21  takano_makoto
  Add Touch Panel Area.

  Revision 1.4  2004/04/07 02:03:17  yada
  fix header comment

  Revision 1.3  2004/03/15 11:02:15  yada
  �C���N���[�h�K�[�h���C��

  Revision 1.2  2004/03/12 12:11:10  yada
  only arrange tab

  Revision 1.1  2004/02/18 07:51:36  yada
  mmap_shared.h �� A7/9���ʂ�

  Revision 1.5  2004/02/18 01:24:57  yada
  XY�{�^�����̈��ǉ�

  Revision 1.4  2004/02/17 06:51:02  yada
  ��蒼���I�ȕ\�L�֕ύX

  Revision 1.3  2004/02/14 06:57:48  yasu
  add HW_COMPONENT_PARAM

  Revision 1.2  2004/02/13 10:39:58  yada
  ARENA���U����X�ɂ��ăC���t�H�G���A�g��

  Revision 1.1  2004/02/13 01:52:31  yada
  ARM7�p�ɒǉ� (temporary?)

  Revision 1.11  2004/02/12 10:56:04  yasu
  new location of include files ARM9/ARM7

  Revision 1.10  2004/02/05 07:09:03  yasu
  change SDK prefix iris -> nitro

  Revision 1.9  2004/01/19 02:29:10  yada
  systemWork �� 0x200��0x280�Ƒ��₵���B
  ����ɔ��������o�̃I�t�Z�b�g�������ύX�ɁB

  Revision 1.8  2004/01/17 11:43:29  yada
  �A���[�i���ʒu�ύX�ɂ�

  Revision 1.7  2004/01/16 11:49:18  yada
  ���U�[�u�̈��ǉ�

  Revision 1.6  2004/01/16 04:13:19  yada
  �A���[�i��`�C��

  Revision 1.5  2004/01/15 12:14:13  yada
  SystemWork �ɃA���[�i�\���̂����悤�ɂ���

  Revision 1.4  2004/01/07 01:41:34  yada
  arena�p�̃f�t�H���gLo/Hi�l��`

  Revision 1.3  2003/12/25 07:30:42  yada
  �^���[������ɂ��ύX

  Revision 1.2  2003/12/22 13:38:04  yasu
  OS_GetSystemWrok���}�N���� OS_LockWord �� struct �^�O�ŃA�N�Z�X����悤�ύX

  Revision 1.1  2003/12/17 06:05:39  yasu
  mmap_reserved.h �� mmap_tcm.h �� mmap_shared.h �ɕ���

  $NoKeywords: $
 *---------------------------------------------------------------------------*/
#ifndef NITRO_HW_COMMON_MMAP_SHARED_H_
#define NITRO_HW_COMMON_MMAP_SHARED_H_

#ifdef __cplusplus
extern "C" {
#endif

//----------------------------------------------------------------------
// MEMORY MAP of SYSTEM SHARED AREA
//----------------------------------------------------------------------
//
#define HW_RED_RESERVED                 (HW_MAIN_MEM + 0x007ff800)      // maybe change later
#define HW_RED_RESERVED_END             (HW_MAIN_MEM + 0x007ffa00)

/* original ROM-header of NITRO-CARD for downloaded program */
#define HW_CARD_ROM_HEADER_SIZE           0x160
/* maximun size of parameters for downloaded program */
#define HW_DOWNLOAD_PARAMETER_SIZE        0x20

//---- default address for shared memory arena
#define  HW_SHARED_ARENA_LO_DEFAULT       HW_MAIN_MEM_SHARED
#ifdef   HW_RED_RESERVED
# define HW_SHARED_ARENA_HI_DEFAULT       (HW_RED_RESERVED - HW_CARD_ROM_HEADER_SIZE - HW_DOWNLOAD_PARAMETER_SIZE)
#else
# define HW_SHARED_ARENA_HI_DEFAULT       (HW_MAIN_MEM_SYSTEM - HW_CARD_ROM_HEADER_SIZE - HW_DOWNLOAD_PARAMETER_SIZE)
#endif

// 0x027ffa00-0x027ffa7f:(128byte):UNUSED

/* 0x027ffa80-0x027ffbdf:(352byte): original ROM-header */
#define HW_CARD_ROM_HEADER              (HW_MAIN_MEM + 0x007ffa80)

/* 0x027ffbe0-0x027ffbff:( 32byte): parameters for downloaded program */
#define HW_DOWNLOAD_PARAMETER           (HW_MAIN_MEM + 0x007ffbe0)

//-------------------------------- MainMemory shared area
#define HW_MAIN_MEM_SYSTEM_SIZE         0x400
#define HW_MAIN_MEM_SYSTEM              (HW_MAIN_MEM + 0x007ffc00)

#define HW_BOOT_CHECK_INFO_BUF          (HW_MAIN_MEM + 0x007ffc00)      // Boot check info (END-0x400)
#define HW_BOOT_CHECK_INFO_BUF_END      (HW_MAIN_MEM + 0x007ffc20)      //                 (END-0x3e0)

#define HW_RESET_PARAMETER_BUF          (HW_MAIN_MEM + 0x007ffc20)      // reset parameter (END-0x3e0)

// 0x027ffc24-0x027ffc2b:(8byte):UNUSED

#define HW_ROM_BASE_OFFSET_BUF          (HW_MAIN_MEM + 0x007ffc2c)      // ROM offset of own program (END-0x3d4)
#define HW_ROM_BASE_OFFSET_BUF_END      (HW_MAIN_MEM + 0x007ffc30)      //                           (END-0x3d0)

#define HW_CTRDG_MODULE_INFO_BUF        (HW_MAIN_MEM + 0x007ffc30)      // Cartridge module info (END-0x3d0)
#define HW_CTRDG_MODULE_INFO_BUF_END    (HW_MAIN_MEM + 0x007ffc3c)      //                       (END-0x3c4)

#define HW_VBLANK_COUNT_BUF             (HW_MAIN_MEM + 0x007ffc3c)      // VBlank counter (END-0x3c4)

#define HW_WM_BOOT_BUF                  (HW_MAIN_MEM + 0x007ffc40)      // WM buffer for Multi-Boot (END-0x3c0)
#define HW_WM_BOOT_BUF_END              (HW_MAIN_MEM + 0x007ffc80)      //                          (END-0x380)

#define HW_NVRAM_USER_INFO              (HW_MAIN_MEM + 0x007ffc80)      // NVRAM user info (END-0x380)
#define HW_NVRAM_USER_INFO_END          (HW_MAIN_MEM + 0x007ffd80)      //                 (END-0x280)

#define HW_BIOS_EXCP_STACK_MAIN         (HW_MAIN_MEM + 0x007ffd80)      // MAINP�f�o�b�K���j�^��O�n���h�� (END-0x280)
#define HW_BIOS_EXCP_STACK_MAIN_END     (HW_MAIN_MEM + 0x007ffd9c)      //                                 (END-0x264)
#define HW_EXCP_VECTOR_MAIN             (HW_MAIN_MEM + 0x007ffd9c)      // MAINP�pHW_EXCP_VECTOR_BUF       (END-0x264)

#define HW_ARENA_INFO_BUF               (HW_MAIN_MEM + 0x007ffda0)      // �A���[�i���\����(27F_FDA0�`27F_FDE7) (END-0x260)
#define HW_REAL_TIME_CLOCK_BUF          (HW_MAIN_MEM + 0x007ffde8)      // RTC

#define HW_DMA_CLEAR_DATA_BUF           (HW_MAIN_MEM + 0x007ffdf0)      // DMA�N���A�f�[�^�E�o�b�t�@ (END-0x210)
#define HW_DMA_CLEAR_DATA_BUF_END       (HW_MAIN_MEM + 0x007ffe00)      //  (���̗̈�� ARM9-TEG �ɂ̂ݎg�p����)

#define HW_ROM_HEADER_BUF               (HW_MAIN_MEM + 0x007ffe00)      // �q�n�l���o�^�G���A�f�[�^�E�o�b�t�@ (END-0x200)
#define HW_ROM_HEADER_BUF_END           (HW_MAIN_MEM + 0x007fff60)      //                                    (END-0x0a0)
#define HW_ISD_RESERVED                 (HW_MAIN_MEM + 0x007fff60)      // IS DEBUGGER Reserved (END-0xa0)
#define HW_ISD_RESERVED_END             (HW_MAIN_MEM + 0x007fff80)      //                      (END-0x80)

#define HW_PXI_SIGNAL_PARAM_ARM9        (HW_MAIN_MEM + 0x007fff80)      // PXI Signal Param for ARM9
#define HW_PXI_SIGNAL_PARAM_ARM7        (HW_MAIN_MEM + 0x007fff84)      // PXI Signal Param for ARM7
#define HW_PXI_HANDLE_CHECKER_ARM9      (HW_MAIN_MEM + 0x007fff88)      // PXI Handle Checker for ARM9
#define HW_PXI_HANDLE_CHECKER_ARM7      (HW_MAIN_MEM + 0x007fff8c)      // PXI Handle Checker for ARM7

#define HW_MIC_LAST_ADDRESS             (HW_MAIN_MEM + 0x007fff90)      // MIC �ŐV�T���v�����O�f�[�^�i�[�A�h���X
#define HW_MIC_SAMPLING_DATA            (HW_MAIN_MEM + 0x007fff94)      // MIC �P�̃T���v�����O����

#define HW_WM_CALLBACK_CONTROL          (HW_MAIN_MEM + 0x007fff96)      // WM �R�[���o�b�N�����p�p�����[�^
#define HW_WM_RSSI_POOL                 (HW_MAIN_MEM + 0x007fff98)      // WM ��M���x�ɂ�闐����

#define HW_SET_CTRDG_MODULE_INFO_ONCE   (HW_MAIN_MEM + 0x007fff9a)      // set ctrdg module info flag
#define HW_IS_CTRDG_EXIST               (HW_MAIN_MEM + 0x007fff9b)      // ctrdg exist flag

#define HW_COMPONENT_PARAM              (HW_MAIN_MEM + 0x007fff9c)      // Component �����p�p�����[�^

#define HW_THREADINFO_MAIN              (HW_MAIN_MEM + 0x007fffa0)      // ThreadInfo for Main processor
#define HW_THREADINFO_SUB               (HW_MAIN_MEM + 0x007fffa4)      // ThreadInfo for Sub  processor
#define HW_BUTTON_XY_BUF                (HW_MAIN_MEM + 0x007fffa8)      // buffer for X and Y button
#define HW_TOUCHPANEL_BUF               (HW_MAIN_MEM + 0x007fffaa)      // buffer for touch panel
#define HW_AUTOLOAD_SYNC_BUF            (HW_MAIN_MEM + 0x007fffae)      // buffer for autoload sync

#define HW_LOCK_ID_FLAG_MAIN            (HW_MAIN_MEM + 0x007fffb0)      // lockID flag for Main processor
#define HW_LOCK_ID_FLAG_SUB             (HW_MAIN_MEM + 0x007fffb8)      // lockID flag for Sub processor

// SpinLock Mutex
#define HW_VRAM_C_LOCK_BUF              (HW_MAIN_MEM + 0x007fffc0)      // �u�q�`�l�|�b�E���b�N�o�b�t�@ (END-0x40)
#define HW_VRAM_D_LOCK_BUF              (HW_MAIN_MEM + 0x007fffc8)      // �u�q�`�l�|�c�E���b�N�o�b�t�@ (END-0x38)
#define HW_WRAM_BLOCK0_LOCK_BUF         (HW_MAIN_MEM + 0x007fffd0)      // �b�o�t�������[�N�q�`�l�E�u���b�N�O�E���b�N�o�b�t�@ (END-0x30)
#define HW_WRAM_BLOCK1_LOCK_BUF         (HW_MAIN_MEM + 0x007fffd8)      // �b�o�t�������[�N�q�`�l�E�u���b�N�P�E���b�N�o�b�t�@ (END-0x28)
#define HW_CARD_LOCK_BUF                (HW_MAIN_MEM + 0x007fffe0)      // �J�[�h�E���b�N�o�b�t�@ (END-0x20)
#define HW_CTRDG_LOCK_BUF               (HW_MAIN_MEM + 0x007fffe8)      // �J�[�g���b�W�E���b�N�o�b�t�@ (END-0x18)
#define HW_INIT_LOCK_BUF                (HW_MAIN_MEM + 0x007ffff0)      // ���������b�N�o�b�t�@ (END-0x10)

#define HW_MMEMCHECKER_MAIN             (HW_MAIN_MEM + 0x007ffff8)      // MainMomory Size Checker for Main processor (END-8)
#define HW_MMEMCHECKER_SUB              (HW_MAIN_MEM + 0x007ffffa)      // MainMomory Size Checker for Sub processor  (END-6)

#define HW_CMD_AREA                     (HW_MAIN_MEM + 0x007ffffe)      // ���C���������R�}���h���s�G���A�i�g�p�֎~�G���A�j(END-2)

//--------------------------------------------------------------------
//---- Lock area
#define HW_SHARED_LOCK_BUF              (HW_VRAM_C_LOCK_BUF)
#define HW_SHARED_LOCK_BUF_END          (HW_INIT_LOCK_BUF + 8)

#define HW_CHECK_DEBUGGER_SW     0x027ffc10     // (u16)debugger check switch. if 0 check buf1, else buf2.
#define HW_CHECK_DEBUGGER_BUF1   0x027ff814     // (u16)debugger checker. 1 if run on debugger.
#define HW_CHECK_DEBUGGER_BUF2   0x027ffc14     // (u16)debugger checker. 1 if run on debugger.


#ifdef __cplusplus
} /* extern "C" */
#endif
/* NITRO_HW_COMMON_MMAP_SHARED_H_ */
#endif
