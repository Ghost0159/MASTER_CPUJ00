/*---------------------------------------------------------------------------*
  Project:  NitroSDK - MB - include
  File:     mb_child.h

  Copyright 2003-2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: mb_child.h,v $
  Revision 1.28  2006/01/18 02:11:30  kitase_hirotake
  do-indent

  Revision 1.27  2005/03/01 01:57:00  yosizaki
  copyright �̔N���C��.

  Revision 1.26  2005/02/28 05:26:25  yosizaki
  do-indent.

  Revision 1.25  2004/11/22 12:53:17  takano_makoto
  MbGameInfoXXX�֘A�̌^��`�A�֐���`�𑼂ֈړ�

  Revision 1.24  2004/10/28 11:01:42  yada
  just fix comment

  Revision 1.23  2004/10/21 08:52:09  takano_makoto
  �R�����g�̊ԈႢ�C��

  Revision 1.22  2004/09/28 12:50:06  yosiokat
  IPL�u�����`1.17.2.4�ƃ}�[�W�B

  Revision 1.17.2.4  2004/09/28 02:38:56  yosiokat
  MbGameInfoRecvList��beaconNo��sameBeaconRecvCount��ǉ��B

  Revision 1.21  2004/09/20 10:39:13  yosiokat
  IPL�u�����`Rev.1.17.2.3�Ƃ̃}�[�W�B

  Revision 1.17.2.3  2004/09/20 10:17:25  yosiokat
  MbGameInfoRecvList����seqNoFixed, seqNoVolat���폜�B�i��M���ł́A��gameInfo����seqNoFixed, seqNoVolat���g�p����悤�C���B�j

  Revision 1.17.2.2  2004/09/18 08:46:19  miya
  HEAD�ƃ}�[�W

  Revision 1.20  2004/09/16 12:51:02  sato_masaki
  - MBi_***�Œ�`���Ă����֐��A�^���AMB_***�ɕύX�B

  Revision 1.19  2004/09/16 12:20:54  miya
  �t�@�C���X�V�p�̉���

  Revision 1.18  2004/09/16 08:43:32  miya
  �t�@�C���X�V�p�����o�ǉ�

  Revision 1.17  2004/09/15 06:55:38  yosiokat
  MbBeaconRecvStatus��u16 usefulGameInfoFlag��ǉ��B

  Revision 1.16  2004/09/13 13:18:10  sato_masaki
  modify pad bytes

  Revision 1.15  2004/09/13 09:25:39  yosiokat
  �E�X�L�������b�N���Ƀ��b�N�Ώۂ��S��������Ȃ���Ԃ̑΍�̂��߁AMbBeaconRecvStatus��notFoundLockTargetCount��ǉ��B
  �EMbGameInfoRecvList.lifeTimeCount��s16�ɏC���B

  Revision 1.14  2004/09/13 08:11:41  sato_masaki
  MBiError, MBiErrorStatus are moved to mb.h

  Revision 1.13  2004/09/13 02:56:58  sato_masaki
  MBiErrorStatus, MBiError��ǉ��B

  Revision 1.12  2004/09/11 12:39:27  yosiokat
  �E���b�N���ԁA�e�@�������̃J�E���g�����C���ɔ����AMbBeaconRecvStatus�̗v�f��ύX�B

  Revision 1.11  2004/09/11 11:13:06  sato_masaki
  - �q�@�p�o�b�t�@�T�C�Y�AMB_CHILD_SYSTEM_BUF_SIZE ���`�B
  - �g�p���[�N�������T�C�Y�擾�֐��AMB_GetChildSystemBufSize()�ǉ��B

  Revision 1.10  2004/09/10 06:32:13  sato_masaki
  comment�ǉ��B

  Revision 1.9  2004/09/09 13:52:26  sato_masaki
  �ʐM���x�\��linkLevel�̒ǉ��B

  Revision 1.8  2004/09/09 10:05:10  sato_masaki
  MBCommCState��GameInfo�֘A�̍��ڒǉ��B

  Revision 1.7  2004/09/09 04:34:56  yosiokat
  MB_CARD_ROM_HEADER_ADDRESS�̒ǉ��B

  Revision 1.6  2004/09/08 12:52:22  sato_masaki
  MB_COMM_CSTATE_ERROR��ǉ��B

  Revision 1.5  2004/09/07 04:34:22  sato_masaki
  small fix

  Revision 1.4  2004/09/06 11:12:25  sato_masaki
  �}���`�u�[�gRAM�A�h���X��`(�����g�p)��mb_private.h����ړ��B

  Revision 1.3  2004/09/04 09:55:26  sato_masaki
  mb.h�Ɏc���Ă����A�q�@�p��`��ǉ��B

  Revision 1.2  2004/09/03 12:05:13  sato_masaki
  comment�C��

  Revision 1.1  2004/09/03 07:06:28  sato_masaki
  �t�@�C�����@�\�ʂɕ����B


  $NoKeywords: $
 *---------------------------------------------------------------------------*/

/* ==========================================================================

    MB���C�u�����̎q�@�p�w�b�_�ł��B
    �}���`�u�[�g�q�@�����IPL�ɂē����C�u�������g�p����ꍇ�́A
    nitro/mb.h�ɉ����A���̃w�b�_���C���N���[�h���Ă��������B

   ==========================================================================*/


#ifndef _MB_CHILD_H_
#define _MB_CHILD_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <nitro/types.h>
#include <nitro/memorymap.h>
#include <nitro/mb.h>
#include "mb_fileinfo.h"
#include "mb_gameinfo.h"

/* ---------------------------------------------------------------------

        define�萔

   ---------------------------------------------------------------------*/

#define MB_CHILD_SYSTEM_BUF_SIZE        (0x6000)

#define MB_MAX_SEND_BUFFER_SIZE         (0x400)
#define MB_MAX_RECV_BUFFER_SIZE         (0x80)

/* ----------------------------------------------------------------------------

    �}���`�u�[�gRAM�A�h���X��`(�����g�p)

   ----------------------------------------------------------------------------*/

/* 
    �}���`�u�[�g�v���O�����z�u�\�͈�

    �����}���`�u�[�g�ɂ�����ARM9�R�[�h�̏������[�h�ő�T�C�Y��2.5MB�ł��B
    
    ARM9�R�[�h�̃��[�h�\�̈��
    MB_LOAD_AREA_LO �` MB_LOAD_AREA_HI
    �͈̔͂ł��B
 */
#define MB_LOAD_AREA_LO                         ( HW_MAIN_MEM )
#define MB_LOAD_AREA_HI                         ( HW_MAIN_MEM + 0x002c0000 )
#define MB_LOAD_MAX_SIZE                        ( MB_LOAD_AREA_HI - MB_LOAD_AREA_LO )


/*
    �}���`�u�[�g����ARM7 static�̎q�@���ł̈ꎞ��M�o�b�t�@�A�h���X��`
    
    ARM7�R�[�h�̃��[�h�\�̈��

    a) 0x02000000 �` 0x02300000
       (MB_LOAD_AREA_LO �` MB_ARM7_STATIC_RECV_BUFFER_END)
    b) 0x02300000 �` 0x023fe000
       (MB_ARM7_STATIC_RECV_BUFFER_END �` 0x023fe000)
    c) 0x037f8000 �` 0x0380f000
       (����WRAM)

    �̂����ꂩ�ɂȂ�܂��B
    
    ������
    ARM7�R�[�h�����WRAM�������� 0x02300000 �ȍ~�Ƀ��[�h����ꍇ�A

    0x022c0000 �` 0x02300000
    (MB_LOAD_AREA_HI �` MB_ARM7_STATIC_RECV_BUFFER_END)
    �Ƀo�b�t�@��݂��A���̃o�b�t�@�Ɏ�M�������̂���
    �u�[�g���Ɏw��A�h���X�ɍĔz�u����悤�ɂ��Ă��܂��B
    
    �E0x02300000 �̃A�h���X���܂����z�u��ARM7�R�[�h
    �E0x02300000 �ȍ~�̃A�h���X�ŁA�T�C�Y�� 0x40000 ( MB_ARM7_STATIC_RECV_BUFFER_SIZE ) 
    �@�𒴂���ARM7�R�[�h�͐��퓮���ۏ؂ł��܂���̂ŁA
    �@���̂悤�ȃR�[�h�̍쐬���֎~�������܂��B
 */

#define MB_ARM7_STATIC_RECV_BUFFER              MB_LOAD_AREA_HI
#define MB_ARM7_STATIC_RECV_BUFFER_END          ( HW_MAIN_MEM + 0x00300000 )
#define MB_ARM7_STATIC_RECV_BUFFER_SIZE         ( MB_ARM7_STATIC_RECV_BUFFER_END - MB_LOAD_AREA_HI )
#define MB_ARM7_STATIC_LOAD_AREA_HI             ( MB_LOAD_AREA_HI + MB_ARM7_STATIC_RECV_BUFFER_SIZE )
#define MB_ARM7_STATIC_LOAD_WRAM_MAX_SIZE       ( 0x18000 - 0x1000 )    // 0x18000 = HW_WRAM_SIZE + HW_PRV_WRAM_SIZE

/* BssDesc�̈ꎞ�i�[�ꏊ */
#define MB_BSSDESC_ADDRESS                      ( HW_MAIN_MEM + 0x003fe800 )
/* MBDownloadFileInfo�̈ꎞ�i�[�ꏊ */
#define MB_DOWNLOAD_FILEINFO_ADDRESS            ( MB_BSSDESC_ADDRESS + ( ( ( MB_BSSDESC_SIZE ) + (32) - 1 ) & ~((32) - 1) ) )
/* �}���`�u�[�g���N���������_�ō������Ă���ROM�J�[�h��ROM�w�b�_�ޔ��ꏊ */
#define MB_CARD_ROM_HEADER_ADDRESS              ( MB_DOWNLOAD_FILEINFO_ADDRESS + ( ( ( MB_DOWNLOAD_FILEINFO_SIZE ) + (32) - 1 ) & ~((32) - 1) ) )
/* ROM Header�̊i�[�ꏊ */
#define MB_ROM_HEADER_ADDRESS                   HW_ROM_HEADER_BUF


/* ---------------------------------------------------------------------

        �}���`�u�[�g���C�u����(MB)�q�@�pAPI. IPL �Ŏg�p����܂�

   ---------------------------------------------------------------------*/

/* �q�@�p�����[�^�ݒ� & �X�^�[�g */
int     MB_StartChild(void);

/* MB�q�@�Ŏg�p���郏�[�N�������̃T�C�Y���擾 */
int     MB_GetChildSystemBufSize(void);

/* �q�@��Ԓʒm�R�[���o�b�N�̐ݒ� */
void    MB_CommSetChildStateCallback(MBCommCStateCallbackFunc callback);

/* �q�@��Ԃ̎擾 */
int     MB_CommGetChildState(void);

/* �_�E�����[�h�J�n */
BOOL    MB_CommStartDownload(void);

/* �q�@�̃_�E�����[�h�i���󋵂� 0 �` 100 �Ŏ擾 */
u16     MB_GetChildProgressPercentage(void);

/* �e�@�ւ̐ڑ����݁��_�E�����[�h�t�@�C���v�� */
int     MB_CommDownloadRequest(int index);      // MbBeaconRecvStatus�̃��X�gNo.�Őڑ��v������e�@���w��B

#ifdef __cplusplus
}
#endif

#endif /*  _MB_CHILD_H_    */
