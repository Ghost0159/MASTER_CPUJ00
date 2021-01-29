/*---------------------------------------------------------------------------*
  Project:  NitroSDK - MB - include
  File:     mb_private.h

  Copyright 2003-2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: mb_private.h,v $
  Revision 1.38  2006/01/18 02:11:30  kitase_hirotake
  do-indent

  Revision 1.37  2005/11/07 01:22:05  okubata_ryoma
  SDK_STATIC_ASSERT����SDK_COMPILER_ASSERT�ɕύX

  Revision 1.36  2005/02/28 05:26:25  yosizaki
  do-indent.

  Revision 1.35  2005/02/21 00:39:34  yosizaki
  replace prefix MBw to MBi.

  Revision 1.34  2005/02/20 23:53:07  takano_makoto
  invalidate PRINT_DEBUG

  Revision 1.33  2005/02/18 11:56:03  yosizaki
  fix around hidden warnings.

  Revision 1.32  2005/01/27 07:05:32  takano_makoto
  fix copyright header.

  Revision 1.31  2005/01/27 07:03:56  takano_makoto
  MB_CommPWork�\���̂�useWvrFlag�����o��ǉ�

  Revision 1.30  2004/11/22 12:52:13  takano_makoto
  mb_block mb_gameinfo���̍\���̂ɕ����E�B���ł�����𐮗�

  Revision 1.29  2004/11/11 11:52:31  yosizaki
  add structures for task-thread.

  Revision 1.28  2004/11/10 13:14:20  yosizaki
  add fileinfo members for cache-system.

  Revision 1.27  2004/10/28 11:02:11  yada
  just fix comment

  Revision 1.26  2004/10/18 11:44:30  yosizaki
  add MB_StartParentEx, MB_EndEx.

  Revision 1.25  2004/10/15 08:50:34  yosizaki
  add start_mp_busy.

  Revision 1.24  2004/10/05 10:05:49  sato_masaki
  BOOT_REQ���󂯂�����MB�I�����������ʂ��鏈���̒ǉ��B

  Revision 1.23  2004/09/25 09:36:52  sato_masaki
  MB_CommGetChildUser()�ɂāA�q�@�����o�b�t�@�ɃR�s�[����悤�ɕύX�B

  Revision 1.22  2004/09/25 04:40:34  sato_masaki
  NitroSDK_IPL_branch 1.14.2.5�@�ƃ}�[�W

  Revision 1.21  2004/09/24 07:30:57  sato_masaki
  small fix

  Revision 1.20  2004/09/24 01:42:39  sato_masaki
  req_data_buf�̃T�C�Y�Ɋւ���s��C���B

  Revision 1.19  2004/09/22 09:42:27  sato_masaki
  MB_MAX_BLOCK�̒l�ύX�B

  Revision 1.18  2004/09/20 12:52:54  sato_masaki
  MB_CommPWork, MB_CommCWork�̕��т�ύX�B

  Revision 1.17  2004/09/17 11:38:10  sato_masaki
  add 'isMbInitialized'�@member to MB_CommCommonWork structure

  Revision 1.16  2004/09/17 04:03:05  miya
  �s�v�ȕ����̍폜

  Revision 1.15  2004/09/16 08:43:29  miya
  �t�@�C���X�V�p�����o�ǉ�

  Revision 1.14  2004/09/15 06:23:32  sato_masaki
  MB_CommCommonWork�ɂ����āAMB_COMM_P_SENDLEN, MB_COMM_C_SENDLEN �̒�`�Ɉˑ����Ă����l���i�[����ϐ���ǉ��B
    (block_size_max, req_data_piece_size, req_data_piece_num)

  Revision 1.13  2004/09/14 13:19:50  sato_masaki
  ���N�G�X�g�f�[�^��f�Љ����đ���悤�ɕύX�B

  Revision 1.12  2004/09/13 13:18:10  sato_masaki
  modify pad bytes

  Revision 1.11  2004/09/11 12:41:42  yosiokat
  �E���b�N���ԁA�e�@�������̃J�E���g�����C���ɔ����AMB_SCAN_TIME_NORMAL�AMB_SCAN_TIME_LOCKING��ǉ��B

  Revision 1.10  2004/09/11 11:08:49  sato_masaki
  MB_CommWork�\���̂�MB_CommPWork, MB_CommCWork, MB_CommCommonWork�ɕ����B

  Revision 1.9  2004/09/10 07:22:47  sato_masaki
  debug swith off

  Revision 1.8  2004/09/10 02:59:10  sato_masaki
  PRINT_DEBUG, CALLBACK_WM_STATE��`�Ɋւ���ύX�B

  Revision 1.7  2004/09/09 09:17:00  sato_masaki
  CALLBACK_WM_STATE���`

  Revision 1.6  2004/09/07 04:48:21  sato_masaki
  padding�����B

  Revision 1.5  2004/09/07 04:26:17  sato_masaki
  rom_header.h��mb_rom_header.h�Ƀ��l�[���B

  Revision 1.4  2004/09/06 11:12:38  sato_masaki
  �}���`�u�[�gRAM�A�h���X��`(�����g�p)��mb_child.h�ֈړ��B

  Revision 1.3  2004/09/04 11:03:50  sato_masaki
  small fix

  Revision 1.2  2004/09/04 09:59:03  sato_masaki
  �L�q�����B

  Revision 1.1  2004/09/03 07:06:28  sato_masaki
  �t�@�C�����@�\�ʂɕ����B


  $NoKeywords: $
 *---------------------------------------------------------------------------*/

/*
    MB���C�u�����̊e�\�[�X�t�@�C���ł́A���̃w�b�_�t�@�C���݂̂��C���N���[�h���Ă��܂��B
*/

#ifndef _MB_PRIVATE_H_
#define _MB_PRIVATE_H_

#ifdef __cplusplus
extern "C" {
#endif

#if	!defined(NITRO_FINALROM)
/*
#define PRINT_DEBUG
*/
#endif

#define CALLBACK_WM_STATE       0

#include <nitro/types.h>
#include <nitro/wm.h>
#include <nitro/mb.h>
#include "mb_wm_base.h"
#include "mb_common.h"
#include "mb_block.h"
#include "mb_rom_header.h"
#include "mb_gameinfo.h"
#include "mb_fileinfo.h"
#include "mb_child.h"

/* �L���b�V�������̂��߂̐V�@�\ */
#include "mb_cache.h"
#include "mb_task.h"

/* �f�o�b�O�o�� */
#ifdef  PRINT_DEBUG
#define MB_OUTPUT( ... )        OS_TPrintf( __VA_ARGS__ )
#define MB_DEBUG_OUTPUT( ... )  MBi_DebugPrint(__FILE__, __LINE__, __FUNCTION__, __VA_ARGS__ )

#else
#define MB_OUTPUT( ... )        ((void)0)
#define MB_DEBUG_OUTPUT( ... )  ((void)0)
#endif

// ----------------------------------------------------------------------------
// definition

/* ----------------------------------------------------------------------------

    �u���b�N�]���֘A��`

   ----------------------------------------------------------------------------*/

#define MB_IPL_VERSION                          (0x0001)
#define MB_MAX_BLOCK                            12288   // max 12288 * 256 = 3.0Mbyte

#define MB_NUM_PARENT_INFORMATIONS              16      /* �ő�e�@�� */

#define MB_SCAN_TIME_NORMAL                     10
#define MB_SCAN_TIME_LOCKING                    220

//SDK_COMPILER_ASSERT( sizeof(MBDownloadFileInfo) <= MB_COMM_BLOCK_SIZE );


/* ----------------------------------------------------------------------------

    �����g�p�^��`

   ----------------------------------------------------------------------------*/

/*
 * MB ���C�u�����Ŏg�p���郏�[�N�̈�.
 */

typedef struct MB_CommCommonWork
{
    //  --- ����    ---
    //  ����M�o�b�t�@
    u32     sendbuf[MB_MAX_SEND_BUFFER_SIZE / sizeof(u32)] ATTRIBUTE_ALIGN(32);
    u32     recvbuf[MB_MAX_RECV_BUFFER_SIZE * MB_MAX_CHILD * 2 / sizeof(u32)] ATTRIBUTE_ALIGN(32);
    MBUserInfo user;                   // ���@�̃��[�U�[���
    u16     isMbInitialized;
    int     block_size_max;
    BOOL    start_mp_busy;             /* WM_StartMP() is operating */
    BOOL    is_started_ex;             /* WM_StartParentEX() called */
    u8      padding[28];
}
MB_CommCommonWork;

typedef struct MB_CommPWork
{
    MB_CommCommonWork common;

    //  --- �e�@�p  ---
    MBUserInfo childUser[MB_MAX_CHILD] ATTRIBUTE_ALIGN(4);
    u16     childversion[MB_MAX_CHILD]; // �q�@�̃o�[�W�������(�q�@�䐔��)
    u32     childggid[MB_MAX_CHILD];   // �q�@��GGID(�q�@�䐔��)
    MBCommPStateCallback parent_callback;       // �e�@�̏�ԃR�[���o�b�N�֐��|�C���^
    int     p_comm_state[MB_MAX_CHILD]; // ���ꂼ��̎q�@�ɑ΂���state������
    u8      file_num;                  // �o�^����Ă���t�@�C����
    u8      cur_fileid;                // ���ݑ��M����File�ԍ�
    s8      fileid_of_child[MB_MAX_CHILD];      // �q�@�̃��N�G�X�g���Ă���File�ԍ�(���N�G�X�g�����̏ꍇ��-1)
    u8      child_num;                 // �G���g���[�q�@��
    u16     child_entry_bmp;           // �q�@�̃G���g���[�Ǘ�bitmap
    MbRequestPieceBuf req_data_buf;    // �q�@�̕������N�G�X�g��M�p�̃o�b�t�@
    u16     req2child[MB_MAX_CHILD];   // �q�@�֑��M���郊�N�G�X�g
    MBUserInfo childUserBuf;           // �A�v���P�[�V�����ɓn�����߂́A�q�@���o�b�t�@

    //  �t�@�C�����
    struct
    {
        // DownloadFileInfo�̃o�b�t�@
        MBDownloadFileInfo dl_fileinfo;
        MBGameInfo game_info;
        MB_BlockInfoTable blockinfo_table;
        MBGameRegistry *game_reg;
        void   *src_adr;               //  �e�@��̃u�[�g�C���[�W�擪�A�h���X
        u16     currentb;              // ���݂̃u���b�N
        u16     nextb;                 // ���񑗐M����u���b�N
        u16     pollbmp;               // �f�[�^���M�pPollBitmap
        u16     gameinfo_child_bmp;    // GameInfo�X�V�̂��߂̌��݂̃����obitmap
        u16     gameinfo_changed_bmp;  // GameInfo�X�V�̂��߂̕ύX���������������obitmap
        u8      active;
        u8      update;

        /*
         * �L���b�V���������[�h�̑Ή��̂��߂ɒǉ�.
         * ����炪�w�����e�� src_adr �̒�.
         */
        MBiCacheList *cache_list;      /* �L���b�V�����X�g */
        u32    *card_mapping;          /* �e�Z�O�����g�擪�� CARD �A�h���X */

    }
    fileinfo[MB_MAX_FILE];

    BOOL    useWvrFlag;                // WVR���g�p���Ă��邩�ǂ����̃t���O
    u8      padding2[20];

    /* �^�X�N�X���b�h�̂��߂ɒǉ� */
    u8      task_work[2 * 1024];
    MBiTaskInfo cur_task;

}
MB_CommPWork;


typedef struct MB_CommCWork
{
    MB_CommCommonWork common;

    //  --- �q�@�p  ---
    WMBssDesc bssDescbuf ATTRIBUTE_ALIGN(32);   // WMBssDesc�̃o�b�N�A�b�v
    MBDownloadFileInfo dl_fileinfo;    // �q�@��DownloadFileInfo�o�b�t�@
    MBCommCStateCallbackFunc child_callback;    // �q�@�̏�ԃR�[���o�b�N�֐��|�C���^
    int     c_comm_state;              // �q�@��state
    int     connectTargetNo;           // �ڑ��Ώۂ�MbBeaconRecvStatus���X�gNo.
    u8      fileid;                    // ���N�G�X�g����File�ԍ�
    u8      _padding1[1];
    u16     user_req;
    u16     got_block;                 // Download�ς݃u���b�N��
    u16     total_block;               // Download�t�@�C�����u���b�N
    u8      recvflag[MB_MAX_BLOCK / 8]; // �u���b�N��M��Ԃ�\���t���O
    MB_BlockInfoTable blockinfo_table;
    int     last_recv_seq_no;          // �O���M�����u���b�N�V�[�P���X�ԍ�
    u8      boot_end_flag;             // BOOT_READY�ɂ��MB�I�����������ۂ��̃t���O
    u8      _padding2[15];
}
MB_CommCWork;


/* ----------------------------------------------------------------------------

    �����g�p�ϐ���`

   ----------------------------------------------------------------------------*/

extern MB_CommCommonWork *mbc;
/* �e�@�p���[�N�ւ̃|�C���^�}�N�� */
#define pPwork                                  ((MB_CommPWork*)mbc)
/* �q�@�p���[�N�ւ̃|�C���^�}�N�� */
#define pCwork                                  ((MB_CommCWork*)mbc)


#ifdef __cplusplus
}
#endif

#endif /*  _MB_PRIVATE_H_  */
